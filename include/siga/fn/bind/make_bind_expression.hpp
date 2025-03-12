#pragma once

#include <siga/fn/bind/stored_func_invoker.hpp>

namespace siga::fn::bind {

template<typename F>
class [[nodiscard]] bind_expression : public stored_func_invoker<F>
{
public:
    using stored_func_invoker<F>::stored_func_invoker;
};

template<typename F>
bind_expression(F) -> bind_expression<F>;

template<typename F>
[[nodiscard]] constexpr auto make_bind_expression(F &&fn)
    noexcept(noexcept(bind_expression(std::forward<F>(fn)))) //
    -> decltype(bind_expression(std::forward<F>(fn)))
{
    return bind_expression(std::forward<F>(fn));
}

} // namespace siga::fn::bind

template<typename F>
class std::is_bind_expression<siga::fn::bind::bind_expression<F>> : public std::true_type
{};
