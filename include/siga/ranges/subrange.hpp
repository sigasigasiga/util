#pragma once

#include <iterator>

#include <siga/meta/tuple.hpp>

namespace siga::ranges {

template<
    meta::pair_like FwdPair,
    typename Pair = std::remove_cvref_t<FwdPair>,
    std::input_iterator First = std::tuple_element_t<0, Pair>,
    std::sentinel_for<First> Second = std::tuple_element_t<1, Pair> >
[[nodiscard]] constexpr auto make_subrange(FwdPair &&p)
{
    return std::ranges::subrange(
        get<0>(std::forward<FwdPair>(p)),
        get<1>(std::forward<FwdPair>(p))
    );
}

} // namespace siga::ranges
