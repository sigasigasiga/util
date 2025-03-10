#pragma once

#include <type_traits>
#include <utility>

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

template<typename DeducedT, typename DesiredT>
concept forward_ref = meta::without_cvref<DesiredT> &&
                      std::convertible_to<std::remove_cvref_t<DeducedT> *, DesiredT *>;

// Implicitly converts `Fwd` to `T` preserving cvref
// Usage:
// ```
// template<forward_ref<std::istream> IstreamFwd>
// void foo(IstreamFwd &&is_fwd)
// {
//     auto &&is = unwrap_forward_ref<std::istream>(std::forward<IstreamFwd>(is_fwd));
//     // ...
// }
// ```
//
// TODO:
// ```
// void foo(forward_ref<std::string> auto &&);
//
// foo(""); // won't compile
// foo(std::string("")); // will compile but it adds inconvenience on the call site
// ```
template<typename T, typename Fwd>
requires forward_ref<Fwd, T>
[[nodiscard]] constexpr meta::copy_cvref_t<Fwd &&, T> unwrap_forward_ref(Fwd &&v) noexcept
{
    static_assert(meta::without_cvref<T>);
    return std::forward<Fwd>(v);
}

// -------------------------------------------------------------------------------------------------

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p0847r7.html#the-shadowing-mitigation-private-inheritance-problem
template<typename To, typename From>
[[nodiscard]] constexpr To private_base_cast(From &from) noexcept
{
    static_assert(std::is_reference_v<To>, "`To` is not a reference");

    static_assert(
        std::is_base_of_v<std::remove_cvref_t<To>, std::remove_cvref_t<From>>,
        "`To` is not base of `From`"
    );

    using cv_from_t = std::remove_reference_t<From>;
    using cv_to_t = std::remove_reference_t<To>;
    static_assert(
        !std::is_const_v<cv_from_t> || std::is_const_v<cv_to_t>,
        "You can't cast `const` away"
    );
    static_assert(
        !std::is_volatile_v<cv_from_t> || std::is_volatile_v<cv_to_t>,
        "You can't cast `volatile` away"
    );

    return (To)from;
}

template<typename To, typename From>
[[nodiscard]] constexpr To private_base_cast(From &&from) noexcept //
    = delete("Casting a temporary to a reference may produce a dangling reference");

} // namespace siga::util
