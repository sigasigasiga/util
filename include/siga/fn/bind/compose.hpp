#pragma once

#include <siga/fn/op/decay_copy.hpp>
#include <siga/fn/op/tuple_fold_invoke.hpp>

namespace siga::fn::bind {

// clang-format off
template<typename... Fs>
[[nodiscard]] constexpr auto compose(Fs &&...fs)
    noexcept(noexcept(std::bind_front(op::tuple_fold_invoke(), std::tuple{std::forward<Fs>(fs)...})))
    -> decltype(std::bind_front(op::tuple_fold_invoke(), std::tuple{std::forward<Fs>(fs)...}))
{
    return std::bind_front(op::tuple_fold_invoke(), std::tuple{std::forward<Fs>(fs)...});
}
// clang-format on

} // namespace siga::fn::bind
