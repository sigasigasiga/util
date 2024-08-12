#pragma once

namespace siga::util {

class first_flag
{
public:
    constexpr first_flag() = default;

public:
    constexpr bool get() noexcept { return std::exchange(flag_, false); }

private:
    bool flag_ = true;
};

} // namespace siga::util
