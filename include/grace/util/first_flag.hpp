#pragma once

#include <utility>

namespace grace::util {

class first_flag
{
public:
    constexpr first_flag() = default;

public:
    [[nodiscard]] constexpr bool get() noexcept { return std::exchange(flag_, false); }
    [[nodiscard]] constexpr bool operator()() noexcept { return get(); }

private:
    bool flag_ = true;
};

} // namespace grace::util
