#pragma once

#include <siga/fn/bind/stored_func_invoker.hpp>

namespace siga::fn::bind {

template<typename F>
class [[nodiscard]] make_bind_expression : public stored_func_invoker<F>
{
public:
    using stored_func_invoker<F>::stored_func_invoker;
};

template<typename F>
make_bind_expression(F) -> make_bind_expression<F>;

// -------------------------------------------------------------------------------------------------

} // namespace siga::fn::bind

template<typename F>
class std::is_bind_expression<siga::fn::bind::make_bind_expression<F>> : public std::true_type
{};
