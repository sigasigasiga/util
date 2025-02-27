#pragma once

#include <iterator>
#include <utility>

#include <siga/util/utility.hpp>

namespace siga::iter {

template<std::input_or_output_iterator It>
[[nodiscard]] constexpr decltype(auto) get_lowest_base(It &&it)
{
    constexpr bool has_base = requires {
        { std::forward<It>(it).base() } -> std::input_or_output_iterator;
    };

    if constexpr(has_base) {
        return (get_lowest_base)(std::forward<It>(it).base());
    } else {
        return util::cut_rvalue_ref(std::forward<It>(it));
    }
}

} // namespace siga::iter
