#pragma once

#include <ranges>

#include <grace/util/utility.hpp>

namespace grace::ranges {

// clang-format off

namespace detail_get_lowest_base {

class adl_tag{};

template<typename Range>
[[nodiscard]] constexpr auto impl(adl_tag, Range &&range, util::overload_priority<0>)
    noexcept(noexcept(util::cut_rvalue_ref(std::forward<Range>(range))))
    -> decltype(util::cut_rvalue_ref(std::forward<Range>(range)))
{
    return util::cut_rvalue_ref(std::forward<Range>(range));
}

template<typename Range>
[[nodiscard]] constexpr auto impl(adl_tag, Range &&range, util::overload_priority<1> = {})
    noexcept(noexcept(impl(adl_tag{}, std::forward<Range>(range).base(), util::overload_priority<1>{})))
    -> decltype(impl(adl_tag{}, std::forward<Range>(range).base(), util::overload_priority<1>{}))
{
    return impl(adl_tag{}, std::forward<Range>(range).base(), util::overload_priority<1>{});
}

} // namespace detail_get_lowest_base

template<std::ranges::range Range>
[[nodiscard]] constexpr auto get_lowest_base(Range &&range)
    noexcept(noexcept(impl(detail_get_lowest_base::adl_tag{}, std::forward<Range>(range))))
    -> decltype(impl(detail_get_lowest_base::adl_tag{}, std::forward<Range>(range)))
{
    return impl(detail_get_lowest_base::adl_tag{}, std::forward<Range>(range));
}

// clang-format on

} // namespace grace::ranges
