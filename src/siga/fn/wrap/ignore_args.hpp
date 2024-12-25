#pragma once

#include <concepts>

#include <siga/util/storage_base.hpp>

namespace siga::fn::wrap {

template<std::invocable F>
class [[nodiscard]] ignore_args : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<typename Self, typename UF = util::copy_cvref_t<Self &&, F>>
    requires std::invocable<UF>
    constexpr decltype(auto) operator()(this Self &&self, auto &&...)
        noexcept(std::is_nothrow_invocable_v<UF>)
    {
        return std::invoke(forward_self<Self, ignore_args>(self).value());
    }
};

template<typename F>
ignore_args(F) -> ignore_args<F>;

} // namespace siga::fn::wrap
