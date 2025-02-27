#pragma once

#include <ranges>

#include <siga/util/utility.hpp>

namespace siga::ranges {

template<std::ranges::range Range>
[[nodiscard]] constexpr decltype(auto) get_lowest_base(Range &&range)
{
    constexpr bool has_base = requires {
        { std::forward<Range>(range).base() } -> std::ranges::range;
    };

    if constexpr(has_base) {
        return (get_lowest_base)(std::forward<Range>(range).base());
    } else {
        return util::cut_rvalue_ref(std::forward<Range>(range));
    }
}

} // namespace siga::ranges
