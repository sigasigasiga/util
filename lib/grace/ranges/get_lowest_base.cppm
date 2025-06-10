module;

#include <ranges>

export module grace.ranges:get_lowest_base;

import grace.meta;
import grace.utility;

using namespace grace;

class adl_tag{};

template<typename Range>
[[nodiscard]] constexpr auto impl(adl_tag, Range &&range, meta::overload_priority<0>)
    noexcept(noexcept(utility::cut_rvalue_ref(std::forward<Range>(range))))
    -> decltype(utility::cut_rvalue_ref(std::forward<Range>(range)))
{
    return utility::cut_rvalue_ref(std::forward<Range>(range));
}

template<typename Range>
[[nodiscard]] constexpr auto impl(adl_tag, Range &&range, meta::overload_priority<1> = {})
    noexcept(noexcept(impl(adl_tag{}, std::forward<Range>(range).base(), meta::overload_priority<1>{})))
    -> decltype(impl(adl_tag{}, std::forward<Range>(range).base(), meta::overload_priority<1>{}))
{
    return impl(adl_tag{}, std::forward<Range>(range).base(), meta::overload_priority<1>{});
}

export namespace grace::ranges {

template<std::ranges::range Range>
[[nodiscard]] constexpr auto get_lowest_base(Range &&range)
    noexcept(noexcept(impl(adl_tag{}, std::forward<Range>(range))))
    -> decltype(impl(adl_tag{}, std::forward<Range>(range)))
{
    return impl(adl_tag{}, std::forward<Range>(range));
}

} // namespace grace::ranges
