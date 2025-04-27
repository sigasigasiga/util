#pragma once

#include <ranges>

#include <grace/meta/tuple.hpp>

namespace grace::ranges {

template<meta::pair_like P>
[[nodiscard]] constexpr auto make_subrange(P &&p)
    noexcept(noexcept(std::ranges::subrange(get<0>(std::forward<P>(p)), get<1>(std::forward<P>(p)))))
    -> decltype(std::ranges::subrange(get<0>(std::forward<P>(p)), get<1>(std::forward<P>(p))))
{
    return std::ranges::subrange(get<0>(std::forward<P>(p)), get<1>(std::forward<P>(p)));
}

} // namespace grace::ranges
