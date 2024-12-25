#pragma once

#include <iterator>
#include <utility>

namespace siga::iter {

template<std::input_or_output_iterator It>
[[nodiscard]] constexpr decltype(auto) get_lowest_iter_base(It &&it)
{
    constexpr bool has_base = requires {
        { std::forward<It>(it).base() } -> std::input_or_output_iterator;
    };

    if constexpr(has_base) {
        return get_lowest_iter_base(std::forward<It>(it).base());
    } else {
        return std::forward<It>(it);
    }
}

template<std::ranges::range Range>
[[nodiscard]] constexpr decltype(auto) get_lowest_range_base(Range &&range)
{
    constexpr bool has_base = requires {
        { std::forward<Range>(range).base() } -> std::ranges::range;
    };

    if constexpr(has_base) {
        return get_lowest_range_base(std::forward<Range>(range).base());
    } else {
        return std::forward<Range>(range);
    }
}

} // namespace siga::iter
