#pragma once

#include <siga/fn/bind/compose.hpp>
#include <siga/fn/op/decay_copy.hpp>
#include <siga/meta/decay_copy.hpp>

namespace siga::fn::wrap::ret {

// useful for `std::optional::transform`
template<typename F>
[[nodiscard]] constexpr auto decay_copy(F &&func)
    noexcept(noexcept(bind::compose(std::forward<F>(func), op::decay_copy()))) //
    -> decltype(bind::compose(std::forward<F>(func), op::decay_copy()))
{
    return bind::compose(std::forward<F>(func), op::decay_copy());
}

} // namespace siga::fn::wrap::ret
