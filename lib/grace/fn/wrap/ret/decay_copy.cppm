module;

#include <utility>

export module grace.fn.wrap.ret:decay_copy;

import grace.fn.bind;
import grace.fn.op;

export namespace grace::fn::wrap::ret {

// useful for `std::optional::transform`
template<typename F>
[[nodiscard]] constexpr auto decay_copy(F &&func)
    noexcept(noexcept(bind::compose(std::forward<F>(func), op::decay_copy())))
    -> decltype(bind::compose(std::forward<F>(func), op::decay_copy()))
{
    return bind::compose(std::forward<F>(func), op::decay_copy());
}

} // namespace grace::fn::wrap::ret
