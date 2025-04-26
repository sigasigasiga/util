#pragma once

#include <functional>

#include <grace/util/storage_base.hpp>

namespace grace::fn::bind {

template<typename F>
class [[nodiscard]] stored_func_invoker : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<
        typename Self,
        typename... Args,
        typename USelf = meta::copy_cvref_t<Self &&, stored_func_invoker>>
    constexpr auto operator()(this Self &&self, Args &&...args) //
        noexcept(noexcept(std::invoke(
            util::private_base_cast<USelf>(self).value(),
            std::forward<Args>(args)...
        ))) //
        -> decltype(std::invoke(
            util::private_base_cast<USelf>(self).value(),
            std::forward<Args>(args)...
        ))
    {
        return std::invoke(
            util::private_base_cast<USelf>(self).value(),
            std::forward<Args>(args)...
        );
    }
};

template<typename F>
stored_func_invoker(F) -> stored_func_invoker<F>;

} // namespace grace::fn::bind
