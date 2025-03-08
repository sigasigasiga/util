#pragma once

#include <functional>

#include <siga/util/storage_base.hpp>

namespace siga::fn::bind {

template<typename F>
class [[nodiscard]] stored_func_invoker : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<
        typename FwdSelf,
        typename... Args,
        typename Self = meta::copy_cvref_t<FwdSelf &&, stored_func_invoker>>
    constexpr auto operator()(this FwdSelf &&self, Args &&...args)
        noexcept(noexcept(std::invoke(((Self)self).value(), std::forward<Args>(args)...))) //
        -> decltype(std::invoke(((Self)self).value(), std::forward<Args>(args)...))
    {
        return std::invoke(((Self)self).value(), std::forward<Args>(args)...);
    }
};

template<typename F>
stored_func_invoker(F) -> stored_func_invoker<F>;

} // namespace siga::fn::bind
