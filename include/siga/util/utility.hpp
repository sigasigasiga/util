#pragma once

#include <type_traits>

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
template<typename From, typename To>
constexpr auto &&forward_self(std::remove_reference_t<From> &self) noexcept
{
    static_assert(meta::without_cvref<To>);
    static_assert(std::is_base_of_v<To, std::remove_cvref_t<From>>);

    using ret_t = meta::copy_cvref_t<From &&, To>;
    return (ret_t)self;
}

template<typename From, typename To>
constexpr auto &&forward_self(std::remove_reference_t<From> &&self) noexcept
{
    static_assert(meta::without_cvref<To>);
    static_assert(std::is_base_of_v<To, std::remove_cvref_t<From>>);
    static_assert(!std::is_lvalue_reference_v<From>, "Cannot forward an rvalue as an lvalue");

    using ret_t = meta::copy_cvref_t<From &&, To>;
    return (ret_t)self;
}

} // namespace siga::util
