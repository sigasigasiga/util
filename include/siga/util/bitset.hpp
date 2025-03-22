#pragma once

#include <bitset>

namespace siga::util {

template<typename... Args, std::size_t Sz = sizeof...(Args)>
[[nodiscard]] constexpr auto make_bitset(Args &&...args)
    noexcept(noexcept(std::bitset<Sz>{(char[]){(std::forward<Args>(args) ? '1' : '0')...}, Sz})) //
    -> decltype(std::bitset<Sz>{(char[]){(std::forward<Args>(args) ? '1' : '0')...}, Sz})
{
    return std::bitset<Sz>{(char[]){(std::forward<Args>(args) ? '1' : '0')...}, Sz};
}

} // namespace siga::util
