#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include <grace/compat/no_unique_address.hpp>
#include <grace/meta/concepts.hpp>
#include <grace/meta/copy_cvref.hpp>

namespace grace::util {

// `util/utility.hpp` sounds funny, but I was trying to somehow resemble the `<utility>` STL header

// https://stackoverflow.com/a/39905208
template<std::uintmax_t P>
class overload_priority : public overload_priority<P - 1>
{};

template<>
class overload_priority<0>
{};

// -------------------------------------------------------------------------------------------------

// clang-format off

// works with explicit copy constructors
template<typename T>
[[nodiscard]] constexpr auto copy(const T &v)
    noexcept(noexcept(T(v)))
    -> decltype(T(v))
{
    return T(v);
}
// clang-format on

// -------------------------------------------------------------------------------------------------

// https://en.cppreference.com/w/cpp/standard_library/decay-copy
// works with explicit copy constructors
template<typename T>
[[nodiscard]] constexpr auto decay_copy(T &&value)
    noexcept(noexcept(std::decay_t<T>(std::forward<T>(value)))) //
    -> decltype(std::decay_t<T>(std::forward<T>(value)))
{
    return std::decay_t<T>(std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

// usage: `cut_rvalue_ref(std::forward<T>(val))`
template<typename T>
[[nodiscard]] auto cut_rvalue_ref(T &&value)                   //
    noexcept(noexcept(static_cast<T>(std::forward<T>(value)))) //
    -> decltype(static_cast<T>(std::forward<T>(value)))
{
    // cut_rvalue_ref(0) -> cut_rvalue_ref<int>(int &&);
    // cut_rvalue_ref(x) -> cut_rvalue_ref<int &>(int &);
    //
    // so if the passed value is lvalue, we return the lvalue,
    // and if the passed value is rvalue, we return the value
    return static_cast<T>(std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr decltype(auto) get_reference(T &&value) noexcept
{
    return std::forward<T>(value);
}

template<typename T>
[[nodiscard]] constexpr decltype(auto) get_reference(std::reference_wrapper<T> ref) noexcept
{
    return ref.get();
}

// -------------------------------------------------------------------------------------------------

// clang-format off

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html#the-shadowing-mitigation-private-inheritance-problem
//
// It is SFINAE-friendly like the builtin casts
template<typename CvRefTo, typename CvFrom, typename CvTo = std::remove_reference_t<CvRefTo>>
requires std::is_reference_v<CvRefTo> &&
         std::is_base_of_v<CvTo, CvFrom> &&
         (!std::is_const_v<CvFrom> || std::is_const_v<CvTo>) &&
         (!std::is_volatile_v<CvFrom> || std::is_volatile_v<CvTo>)
[[nodiscard]] constexpr CvRefTo private_base_cast(CvFrom &from) noexcept
{
    return (CvRefTo)from;
}

// clang-format on

// -------------------------------------------------------------------------------------------------

// 1. I guess there's no way to implement it by writing it three times
// 2. If `To` is `void`, then it is a SFINAE-out case. I'm not sure if this is desired
template<typename To, typename From>
requires std::is_convertible_v<From, To>
[[nodiscard]] constexpr To implicit_cast(From &&from)
    noexcept(std::is_nothrow_convertible_v<From, To>)
{
    return from;
}

} // namespace grace::util
