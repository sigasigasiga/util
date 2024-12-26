#pragma once

#include <siga/fn/bind/compose.hpp>
#include <siga/fn/op/cut_rvalue_ref.hpp>
#include <siga/meta/decay_copy.hpp>

namespace siga::fn::wrap {

// useful for `tl::optional::transform`,
// although i think it should've been replacing `T&&` with `T` automatically
template<typename F>
[[nodiscard]] constexpr auto cut_rvalue_ref(F &&func)
    noexcept(meta::is_nothrow_decay_copy_constructible_v<F>)
{
    return bind::compose(std::forward<F>(func), op::cut_rvalue_ref());
}

} // namespace siga::fn::wrap
