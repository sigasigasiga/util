#pragma once

#include <type_traits>

#include <siga/util/compat.hpp>

namespace siga::util {

// `util/utility.hpp` sounds funny, but I was trying to somehow resemble the `<utility>` STL header

template<typename T>
struct [[nodiscard]] no_unique_address_if_empty
{
    T value_;
};

template<typename T>
requires std::is_empty_v<T>
struct [[nodiscard]] no_unique_address_if_empty<T>
{
    SIGA_UTIL_NO_UNIQUE_ADDRESS T value_;
};

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] copy_t
{
public:
    template<typename T>
    [[nodiscard]] static constexpr T operator()(const T &v)
        noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        return v;
    }
};

inline constexpr copy_t copy;

// -------------------------------------------------------------------------------------------------

template<typename T>
class is_decay_copy_constructible : public std::is_constructible<std::decay_t<T>, T &&>
{};

template<typename T>
constexpr bool is_decay_copy_constructible_v = is_decay_copy_constructible<T>::value;

template<typename T>
class is_trivially_decay_copy_constructible
    : public std::is_trivially_constructible<std::decay_t<T>, T &&>
{};

template<typename T>
constexpr bool is_trivially_decay_copy_constructible_v =
    is_trivially_decay_copy_constructible<T>::value;

template<typename T>
class is_nothrow_decay_copy_constructible
    : public std::is_nothrow_constructible<std::decay_t<T>, T &&>
{};

template<typename T>
constexpr bool is_nothrow_decay_copy_constructible_v =
    is_nothrow_decay_copy_constructible<T>::value;

// unlike `copy_t`, the return value may be move-constructed
// https://en.cppreference.com/w/cpp/standard_library/decay-copy
class [[nodiscard]] decay_copy_t
{
public:
    template<typename T>
    [[nodiscard]] static constexpr std::decay_t<T> operator()(T &&value)
        noexcept(is_nothrow_decay_copy_constructible_v<T>)
    {
        return value;
    }
};

inline constexpr decay_copy_t decay_copy;

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] cut_rvalue_ref_t
{
public:
    template<typename T>
    [[nodiscard]] static constexpr decltype(auto) operator()(T &&value)
        noexcept(std::is_lvalue_reference_v<T> || std::is_nothrow_move_constructible_v<T>)
    {
        // cut_rvalue_ref(0) -> cut_rvalue_ref<int>(int &&);
        // cut_rvalue_ref(x) -> cut_rvalue_ref<int &>(int &);
        //
        // so if the passed value is lvalue, we return the lvalue,
        // and if the passed value is rvalue, we return the value
        return static_cast<T>(std::forward<T>(value));
    }
};

inline constexpr cut_rvalue_ref_t cut_rvalue_ref;

// -------------------------------------------------------------------------------------------------

// useful for `std::optional::transform`
template<typename F>
[[nodiscard]] constexpr auto decay_copy_wrap(F &&func)
{
    return compose(std::forward<F>(func), decay_copy);
}

// -------------------------------------------------------------------------------------------------

// useful for `tl::optional::transform`,
// although i think it should replace `T&&` with `T` automatically
template<typename F>
[[nodiscard]] constexpr auto cut_rvalue_ref_wrap(F &&func)
{
    return compose(std::forward<F>(func), cut_rvalue_ref);
}

} // namespace siga::util
