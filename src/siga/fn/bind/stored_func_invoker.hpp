#pragma once

#include <siga/util/storage_base.hpp>

namespace siga::fn::bind {

template<typename F>
class [[nodiscard]] stored_func_invoker : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<typename Self, typename... Args, typename UF = util::copy_cvref_t<Self, F>>
    requires std::invocable<UF, Args...>
    constexpr decltype(auto) operator()(this Self &&self, Args &&...args)
        noexcept(std::is_nothrow_invocable_v<UF, Args...>)
    {
        return std::invoke(
            util::forward_self<Self, stored_func_invoker>(self).value(),
            std::forward<Args>(args)...
        );
    }
};

template<typename F>
stored_func_invoker(F) -> stored_func_invoker<F>;

} // namespace siga::fn::bind
