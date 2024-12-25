#pragma once

#include <concepts>

#include <siga/util/storage_base.hpp>

namespace siga::fn::wrap {

namespace detail {

template<std::invocable F>
class [[nodiscard]] ignore_args_impl : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<typename Self, typename UF = meta::copy_cvref_t<Self &&, F>>
    requires std::invocable<UF>
    constexpr decltype(auto) operator()(this Self &&self, auto &&...)
        noexcept(std::is_nothrow_invocable_v<UF>)
    {
        return std::invoke(util::forward_self<Self, ignore_args_impl>(self).value());
    }
};

} // namespace detail

template<std::invocable F>
[[nodiscard]] constexpr auto ignore_args(F &&fn)
    noexcept(std::is_nothrow_constructible_v<std::decay_t<F>, F &&>)
{
    return detail::ignore_args_impl<std::decay_t<F>>(std::forward<F>(fn));
}

} // namespace siga::fn::wrap
