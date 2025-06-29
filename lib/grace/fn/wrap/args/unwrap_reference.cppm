module;

#include <functional>
#include <utility>

export module grace.fn.wrap.args:unwrap_reference;

import grace.fn.invoke;

constexpr auto unwrap_reference_op = []<typename... Args>
    (Args &&...args)
    noexcept(noexcept(grace::fn::invoke::unwrap_reference(std::forward<Args>(args)...)))
    -> decltype(grace::fn::invoke::unwrap_reference(std::forward<Args>(args)...))
{
    return grace::fn::invoke::unwrap_reference(std::forward<Args>(args)...);
};

export namespace grace::fn::wrap::args {

template<typename F>
[[nodiscard]] constexpr auto unwrap_reference(F &&fn)
    noexcept(noexcept(std::bind_front(unwrap_reference_op, std::forward<F>(fn))))
    -> decltype(std::bind_front(unwrap_reference_op, std::forward<F>(fn)))
{
    return std::bind_front(unwrap_reference_op, std::forward<F>(fn));
}

} // namespace grace::fn::wrap::args
