#pragma once

#include <siga/fn/op/fold_invoke.hpp>
#include <siga/fn/op/decay_copy.hpp>

namespace siga::fn::bind {

template<typename... Fs>
[[nodiscard]] constexpr auto compose(Fs &&...fs)
    noexcept((... && meta::is_nothrow_decay_copy_constructible_v<Fs>))
{
    return std::bind_front(op::fold_invoke(), std::tuple{std::forward<Fs>(fs)...});
}

} // namespace siga::fn::bind
