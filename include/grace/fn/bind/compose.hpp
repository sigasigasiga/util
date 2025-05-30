#pragma once

#include <grace/fn/op/tuple_fold_invoke.hpp>

namespace grace::fn::bind {

template<typename... Fs>
[[nodiscard]] constexpr auto compose(Fs &&...fs)
    noexcept(noexcept(std::bind_front(op::tuple_fold_invoke(), std::tuple{std::forward<Fs>(fs)...})))
    -> decltype(std::bind_front(op::tuple_fold_invoke(), std::tuple{std::forward<Fs>(fs)...}))
{
    return std::bind_front(op::tuple_fold_invoke(), std::tuple{std::forward<Fs>(fs)...});
}

} // namespace grace::fn::bind
