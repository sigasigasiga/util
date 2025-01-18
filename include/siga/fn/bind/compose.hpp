#pragma once

#include <siga/fn/op/fold_invoke.hpp>
#include <siga/fn/op/decay_copy.hpp>

namespace siga::fn::bind {

template<typename... Fs>
[[nodiscard]] constexpr auto compose(Fs &&...fs)
    noexcept(noexcept(std::bind_front(op::fold_invoke(), std::tuple{std::forward<Fs>(fs)...}))) //
    -> decltype(std::bind_front(op::fold_invoke(), std::tuple{std::forward<Fs>(fs)...}))
{
    return std::bind_front(op::fold_invoke(), std::tuple{std::forward<Fs>(fs)...});
}

} // namespace siga::fn::bind
