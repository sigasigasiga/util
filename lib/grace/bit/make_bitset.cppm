module;

#include <array>
#include <bitset>

export module grace.bit:make_bitset;

export namespace grace::bit {

// the last one in the pack is the first one in the bitset, as in `bitset`'s constructor
// you may use `util::reverse_arguments` to fix that
template<typename... Args, std::size_t Sz = sizeof...(Args)>
[[nodiscard]] constexpr auto make_bitset(Args &&...args)
    noexcept(noexcept(std::bitset<Sz>{std::array{(std::forward<Args>(args) ? '1' : '0')...}.data(), Sz}))
    -> decltype(std::bitset<Sz>{std::array{(std::forward<Args>(args) ? '1' : '0')...}.data(), Sz})
{
    return std::bitset<Sz>{std::array{(std::forward<Args>(args) ? '1' : '0')...}.data(), Sz};
}

} // namespace grace::bit
