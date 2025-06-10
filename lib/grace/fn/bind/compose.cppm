module;

#include <functional>
#include <utility>

export module grace.fn.bind:compose;

import grace.fn.op;

export namespace grace::fn::bind {

template<typename... Fs>
[[nodiscard]] constexpr auto compose(Fs &&...fs)
    noexcept(noexcept(std::bind_front(op::fold(), std::tuple{std::forward<Fs>(fs)...})))
    -> decltype(std::bind_front(op::fold(), std::tuple{std::forward<Fs>(fs)...}))
{
    return std::bind_front(op::fold(), std::tuple{std::forward<Fs>(fs)...});
}

} // namespace grace::fn::bind
