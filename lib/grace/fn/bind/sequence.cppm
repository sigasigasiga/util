module;

#include <functional>
#include <utility>

export module grace.fn.bind:sequence;

import grace.fn.op;

export namespace grace::fn::bind {

template<typename ...Fs>
[[nodiscard]] constexpr auto sequence(Fs &&...fs)
    noexcept(noexcept(std::bind_front(op::sequence(), std::tuple(std::forward<Fs>(fs)...))))
    -> decltype(std::bind_front(op::sequence(), std::tuple(std::forward<Fs>(fs)...)))
{
    return std::bind_front(op::sequence(), std::tuple(std::forward<Fs>(fs)...));
}

} // namespace grace::fn::bind
