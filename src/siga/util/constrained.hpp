#pragma once

#include <concepts>
#include <expected>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <siga/util/storage_base.hpp>

namespace siga::util {

// unsafe tag types ================================================================================

class unsafe_t
{
public:
    explicit unsafe_t() = default;
};

inline constexpr unsafe_t unsafe;

// checker =========================================================================================

template<typename Checker, typename T>
concept constrained_checker_for = requires(const T value, const Checker checker) {
    { checker(value) } -> std::same_as<std::expected<void, typename Checker::error_type>>;
};

// error handler ===================================================================================

template<typename ErrorHandler, typename Checker>
concept error_handler_for =
    requires(const ErrorHandler eh, const typename Checker::error_type err) { eh(err); };

// constrained =====================================================================================

template<typename T, constrained_checker_for<T> Checker, error_handler_for<Checker> ErrorHandler>
class constrained : private storage_base<T>,
                    private storage_base<Checker>,
                    private storage_base<ErrorHandler>
{
public:
    static_assert(std::is_object_v<T>);
    static_assert(std::is_object_v<Checker>);
    static_assert(std::is_object_v<ErrorHandler>);

    static_assert(!std::same_as<T, unsafe_t>);

public:
    using value_type = T;
    using checker_type = Checker;
    using error_handler_type = ErrorHandler;

public: // throwing constructors
    template<
        typename U = value_type,
        typename UChecker = checker_type,
        typename UErrorHandler = error_handler_type>
    constexpr constrained(U value = {}, UChecker &&checker = {}, UErrorHandler &&error_handler = {})
        : storage_base<value_type>(std::forward<U>(value))
        , storage_base<checker_type>(std::forward<UChecker>(checker))
        , storage_base<error_handler_type>(std::forward<UErrorHandler>(error_handler))
    {
        validate();
    }

public: // unsafe constructors
    template<
        typename U = value_type,
        typename UChecker = checker_type,
        typename UErrorHandler = error_handler_type>
    constrained(unsafe_t, U value = {}, UChecker &&checker = {}, UErrorHandler &&error_handler = {})
        : storage_base<value_type>(std::forward<U>(value))
        , storage_base<checker_type>(std::forward<UChecker>(checker))
        , storage_base<error_handler_type>(std::forward<UErrorHandler>(error_handler))
    {
    }

public: // safe assignment
    constrained &operator=(const constrained &) = default;
    constrained &operator=(constrained &&) = default;

public: // const value access
    constexpr const value_type &operator*() const noexcept { return get(); }
    constexpr const value_type *operator->() const noexcept { return std::addressof(get()); }

    constexpr const value_type &get() const noexcept
    {
        assert(check());
        return storage_base<value_type>::value();
    }

    constexpr const checker_type &get_checker() const noexcept
    {
        return storage_base<checker_type>::value();
    }

    constexpr const error_handler_type &get_error_handler() const noexcept
    {
        return storage_base<error_handler_type>::value();
    }

public: // mutable value access
    constexpr value_type release() && noexcept
    {
        assert(check());
        return std::move(storage_base<value_type>::value());
    }

    constexpr T &get(unsafe_t)
    {
        assert(check());
        return storage_base<value_type>::value();
    }

private:
    constexpr auto check() { return get_checker()(get()); }

    constexpr void validate()
    {
        auto result = check();
        if(!result) {
            get_error_handler()(std::move(result).error());
        }
    }
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

} // namespace siga::util
