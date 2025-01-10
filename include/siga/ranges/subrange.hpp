#pragma once

#include <iterator>

namespace siga::ranges {

template<std::input_iterator I, std::sentinel_for<I> S>
[[nodiscard]] constexpr auto make_subrange(std::pair<I, S> p) // TODO: pair-like
{
    return std::ranges::subrange(p.first, p.second);
}

} // namespace siga::ranges
