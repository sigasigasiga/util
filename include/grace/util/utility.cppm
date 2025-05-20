module;

#include <cstdint>
#include <type_traits>
#include <utility>

export module grace.util.utility;

import grace.meta;

export namespace grace::util {

// `util/utility.hpp` sounds funny, but I was trying to somehow resemble the `<utility>` STL header

// https://stackoverflow.com/a/39905208
template<std::uintmax_t P>
class overload_priority : public overload_priority<P - 1>
{};

template<>
class overload_priority<0>
{};

// -------------------------------------------------------------------------------------------------

// works with explicit copy constructors
template<typename T>
[[nodiscard]] constexpr auto copy(const T &v)
    noexcept(noexcept(T(v)))
    -> decltype(T(v))
{
    return T(v);
}

// -------------------------------------------------------------------------------------------------

// https://en.cppreference.com/w/cpp/standard_library/decay-copy
// works with explicit copy constructors
template<typename T>
[[nodiscard]] constexpr auto decay_copy(T &&value)
    noexcept(noexcept(std::decay_t<T>(std::forward<T>(value))))
    -> decltype(std::decay_t<T>(std::forward<T>(value)))
{
    return std::decay_t<T>(std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

// usage: `cut_rvalue_ref(std::forward<T>(val))`
template<typename T>
[[nodiscard]] auto cut_rvalue_ref(T &&value)
    noexcept(noexcept(static_cast<T>(std::forward<T>(value))))
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

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html#the-shadowing-mitigation-private-inheritance-problem
//
// Notes:
// 1. It is SFINAE-friendly like the builtin casts
// 2. It doesn't differentiate between xvalues and prvalues like the builtin casts
//    (e.g. `const_cast<int &&>(std::move(x))` works, but `const_cast<int &&>(3)` doesn't)
template<typename CvRefTo, typename CvFrom, typename CvTo = std::remove_reference_t<CvRefTo>>
requires std::is_reference_v<CvRefTo> &&
         std::is_base_of_v<CvTo, CvFrom> &&
         (!std::is_const_v<CvFrom> || std::is_const_v<CvTo>) &&
         (!std::is_volatile_v<CvFrom> || std::is_volatile_v<CvTo>)
[[nodiscard]] constexpr CvRefTo private_base_cast(CvFrom &from) noexcept
{
    return (CvRefTo)from;
}

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

// -------------------------------------------------------------------------------------------------

// Generic way to implement assignment for any class that has
// 1. `To(FwdFrom &&)` construcotr
// 2. `.swap` method
//
// NB: I don't really like the idea of returning an lvalue reference
//     but I don't like inconsistencies with the standard even more
template<typename FwdTo, typename FwdFrom, typename To = std::remove_cvref_t<FwdTo>>
FwdTo &default_assign(FwdTo &&to, FwdFrom &&from)
    noexcept(noexcept(To(std::forward<FwdFrom>(from)).swap(to)))
    requires requires { To(std::forward<FwdFrom>(from)).swap(to); }
{
    if(std::addressof(to) != std::addressof(from)) {
        To(std::forward<FwdFrom>(from)).swap(to);
    }

    return to;
}

} // namespace grace::util
