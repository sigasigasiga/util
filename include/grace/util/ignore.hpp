#pragma once

namespace grace::util {

class [[nodiscard]] ignore_t
{
public:
    constexpr const ignore_t &operator=(auto &&) const noexcept { return *this; }
    constexpr void operator()(auto &&...) const noexcept {}
};

inline constexpr ignore_t ignore;

} // namespace grace::util
