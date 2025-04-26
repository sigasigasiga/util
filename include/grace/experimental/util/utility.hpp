#pragma once

#include <grace/meta/concepts.hpp>
#include <grace/meta/copy_cvref.hpp>

namespace grace::experimental::util {

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

} // namespace grace::experimental::util
