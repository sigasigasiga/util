#pragma once

#include <functional>

#include <siga/meta/decay_copy.hpp>
#include <siga/util/storage_base.hpp>
#include <siga/util/utility.hpp>

namespace siga::fn::wrap {

namespace detail {

// TODO: make `apply_trait_wrap` and
// implement `unwrap_reference` as `apply_trait_wrap<std::unwrap_reference>`?
template<typename F>
class [[nodiscard]] unwrap_reference_impl : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    // clang-format off
    template<typename Self, typename... Args>
    constexpr auto operator()(this Self &&self, Args &&...args)
        noexcept(noexcept(std::invoke(
            util::forward_self<Self, unwrap_reference_impl>(self).value(),
            util::get_reference(std::forward<Args>(args))...
        )))
        -> decltype(std::invoke(
            util::forward_self<Self, unwrap_reference_impl>(self).value(),
            util::get_reference(std::forward<Args>(args))...
        ))
    {
        return std::invoke(
            util::forward_self<Self, unwrap_reference_impl>(self).value(),
            util::get_reference(std::forward<Args>(args))...
        );
    }
    // clang-format on
};

} // namespace detail

template<typename F>
[[nodiscard]] constexpr auto unwrap_reference(F &&fn)
    noexcept(meta::is_nothrow_decay_copy_constructible_v<F>)
{
    return detail::unwrap_reference_impl<std::decay_t<F>>(std::forward<F>(fn));
}

} // namespace siga::fn::wrap
