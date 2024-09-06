#pragma once

#include <bitset>

namespace siga::util {

template<std::convertible_to<bool>... Ts>
constexpr auto make_bitset(Ts &&...values)
{
    constexpr std::size_t count = sizeof...(Ts);
    std::bitset<count> ret;

    auto impl = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        return (ret.set(Is, static_cast<bool>(values)), ...);
    };

    return impl(std::make_index_sequence<count>{});
}

} // namespace siga::util
