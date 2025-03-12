#pragma once

#include <type_traits>
#include <utility>
#include <version>

#include <siga/compat/no_unique_address.hpp>
#include <siga/meta/concepts.hpp>
#include <siga/meta/copy_cvref.hpp>

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
    SIGA_COMPAT_NO_UNIQUE_ADDRESS T value_;
};

// -------------------------------------------------------------------------------------------------

template<std::size_t P>
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

template<typename To, typename From>
[[nodiscard]] constexpr To private_base_cast(From &&from) noexcept
#if __cpp_deleted_function >= 202403L
    = delete("Casting a temporary to a reference may produce a dangling reference");
#else
    = delete;
#endif

// clang-format on

} // namespace siga::util
