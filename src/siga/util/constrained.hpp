#pragma once

#include <concepts>
#include <expected>
#include <stdexcept>
#include <type_traits>
#include <utility>

// unsafe tag types ================================================================================

class unsafe_t
{
public:
    explicit unsafe_t() = default;
};

inline constexpr unsafe_t unsafe;

class unsafe_piecewise_construct_t
{
public:
    explicit unsafe_piecewise_construct_t() = default;
};

inline constexpr unsafe_piecewise_construct_t unsafe_piecewise_construct;

// checker =========================================================================================

template<typename Checker, typename T>
concept constrained_checker_for = requires(T value, Checker checker) {
    { checker(value) } -> std::same_as<std::expected<void, typename Checker::error_type>>;
};

// error handler ===================================================================================

template<typename ErrorHandler, typename Checker>
concept error_handler_for = requires(ErrorHandler eh, typename Checker::error_type err) { //
    eh(err);
};

// constrained =====================================================================================

template<typename T, constrained_checker_for<T> Checker, error_handler_for<Checker> ErrorHandler>
class constrained
{
public:
    static_assert(std::is_object_v<T>);
    static_assert(std::is_object_v<Checker>);
    static_assert(std::is_object_v<ErrorHandler>);

    static_assert(!std::same_as<T, std::in_place_t>);

    static_assert(!std::same_as<T, unsafe_t>);
    static_assert(!std::same_as<T, unsafe_piecewise_construct_t>);

public:
    using value_type = T;
    using checker_type = Checker;
    using error_handler_type = ErrorHandler;

public: // throwing constructors
    template<
        typename U = value_type,
        typename UChecker = checker_type,
        typename UErrorHandler = error_handler_type>
    constrained(U value = {}, UChecker &&checker = {}, UErrorHandler &&error_handler = {})
        : data_(
              std::forward<U>(value),
              std::forward<UChecker>(checker),
              std::forward<UErrorHandler>(error_handler)
          )
    {
        validate();
    }

public: // unsafe constructors
    template<
        typename U = value_type,
        typename UChecker = checker_type,
        typename UErrorHandler = error_handler_type>
    constrained(unsafe_t, U value = {}, UChecker &&checker = {}, UErrorHandler &&error_handler = {})
        : data_(
              std::forward<U>(value),
              std::forward<UChecker>(checker),
              std::forward<UErrorHandler>(error_handler)
          )
    {
    }

public: // safe assignment
    constrained &operator=(const constrained &) = default;
    constrained &operator=(constrained &&) = default;

public: // const value access
    constexpr const T &get() const
    {
        assert(check());
        return std::get<0>(data_);
    }

    constexpr const value_type &operator*() const noexcept { return get(); }
    constexpr const value_type *operator->() const noexcept { return std::addressof(get()); }

public: // mutable value access
    constexpr value_type release() && noexcept
    {
        assert(check());
        return std::get<0>(std::move(data_));
    }

    constexpr T &get(unsafe_t)
    {
        assert(check());
        return std::get<0>(data_);
    }

public: // checker & error handler access
    // TODO: should it be available via non-const ref?
    template<typename Self>
    constexpr auto &&get_checker(this Self &&self) noexcept
    {
        return std::get<1>(std::forward<Self>(self).data_);
    }

    // TODO: should it be available via non-const ref?
    template<typename Self>
    constexpr auto &&get_error_handler(this Self &&self) noexcept
    {
        return std::get<2>(std::forward<Self>(self).data_);
    }

private:
    constexpr auto check() { return get_checker()(data_); }

    constexpr void validate()
    {
        auto result = check();
        if(!result) {
            get_error_handler()(std::move(result).error());
        }
    }

private:
    std::tuple<T, checker_type, error_handler_type> data_;
};

// example checker which should be probably used with `util::throw_exception`

template<std::integral T>
class non_zero_check
{
public:
    class error_type : public std::logic_error
    {
    public:
        error_type()
            : std::logic_error("non_zero_check")
        {
        }
    };

public:
    static std::expected<void, error_type> operator()(T value)
    {
        std::expected<void, error_type> ret;

        if(value == 0) {
            ret = std::unexpected{error_type{}};
        }

        return ret;
    }
};
