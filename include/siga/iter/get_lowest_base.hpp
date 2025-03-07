#pragma once

#include <iterator>
#include <utility>

#include <siga/util/utility.hpp>

namespace siga::iter {

// clang-format off

namespace detail_get_lowest_base {

class adl_tag{};

template<typename FwdIt>
[[nodiscard]] constexpr auto impl(adl_tag, FwdIt &&it, util::overload_priority<0>)
    noexcept(noexcept(util::cut_rvalue_ref(std::forward<FwdIt>(it))))
    -> decltype(util::cut_rvalue_ref(std::forward<FwdIt>(it)))
{
    return util::cut_rvalue_ref(std::forward<FwdIt>(it));
}

template<typename FwdIt>
[[nodiscard]] constexpr auto impl(adl_tag, FwdIt &&it, util::overload_priority<1> = {})
    noexcept(noexcept(impl(adl_tag{}, std::forward<FwdIt>(it).base(), util::overload_priority<1>{})))
    -> decltype(impl(adl_tag{}, std::forward<FwdIt>(it).base(), util::overload_priority<1>{}))
{
    return impl(adl_tag{}, std::forward<FwdIt>(it).base(), util::overload_priority<1>{});
}

} // namespace detail_get_lowest_base

template<typename FwdIt>
requires std::input_or_output_iterator<std::remove_cvref_t<FwdIt>>
[[nodiscard]] constexpr auto get_lowest_base(FwdIt &&it)
    noexcept(noexcept(impl(detail_get_lowest_base::adl_tag{}, std::forward<FwdIt>(it))))
    -> decltype(impl(detail_get_lowest_base::adl_tag{}, std::forward<FwdIt>(it)))
{
    return impl(detail_get_lowest_base::adl_tag{}, std::forward<FwdIt>(it));
}

// clang-format on

} // namespace siga::iter
