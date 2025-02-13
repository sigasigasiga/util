#pragma once

#include <concepts>
#include <memory>

#include <siga/util/utility.hpp>

namespace siga::util {

namespace detail_to_address {

constexpr overload_priority<3> highest_priority;

class adl_tag
{};

template<typename Ptr>
auto impl(adl_tag, const Ptr &ptr, overload_priority<0>)
requires requires(std::size_t i) {
    // overload for `unique_ptr<T[]>` and `shared_ptr<T[]>`.
    // it isn't perfect by any means but i guess that's something?
    { ptr[i] } -> std::same_as<typename Ptr::element_type &>;
    { ptr.get() } -> std::same_as<typename Ptr::element_type *>;
}
{
    return ptr.get();
}

template<typename Ptr>
auto impl(adl_tag, const Ptr &ptr, overload_priority<1>)
    -> decltype(impl(adl_tag{}, ptr.operator->(), highest_priority))
{
    return impl(adl_tag{}, ptr.operator->(), highest_priority);
}

template<typename Ptr>
auto impl(adl_tag, const Ptr &ptr, overload_priority<2>)
    -> decltype(std::pointer_traits<Ptr>::to_address(ptr))
{
    return std::pointer_traits<Ptr>::to_address(ptr);
}

template<typename T>
T *impl(adl_tag, T *ptr, overload_priority<3>)
{
    return ptr;
}

} // namespace detail_to_address

// clang-format off

// `std::to_address` is not SFINAE-friendly and it doesn't support `{unique,shared}_ptr<T[]>`
auto to_address(const auto &ptr)
    noexcept(noexcept(impl(detail_to_address::adl_tag{}, ptr, detail_to_address::highest_priority)))
    -> decltype(impl(detail_to_address::adl_tag{}, ptr, detail_to_address::highest_priority))
{
    return impl(detail_to_address::adl_tag{}, ptr, detail_to_address::highest_priority);
}
// clang-format on

} // namespace siga::util
