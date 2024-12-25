#pragma once

#include <siga/fn/bind/compose.hpp>
#include <siga/fn/op/decay_copy.hpp>

namespace siga::fn::wrap {

// useful for `std::optional::transform`
template<typename F>
[[nodiscard]] constexpr auto decay_copy(F &&func)
{
    return bind::compose(std::forward<F>(func), op::decay_copy());
}

} // namespace siga::fn::wrap
