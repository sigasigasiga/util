module;

#include <concepts>
#include <memory>

export module grace.memory:to_address;

import grace.meta;

namespace detail_to_address {

template<std::uintmax_t N>
using p = grace::meta::overload_priority<N>;

class adl_tag
{};

template<typename Ptr>
constexpr auto impl(adl_tag, const Ptr &ptr, p<0>)
    noexcept(noexcept(impl(adl_tag{}, ptr.operator->(), p<2>{})))
    -> decltype(impl(adl_tag{}, ptr.operator->(), p<2>{}))
{
    return impl(adl_tag{}, ptr.operator->(), p<2>{});
}

template<typename Ptr>
constexpr auto impl(adl_tag, const Ptr &ptr, p<1>)
    noexcept(noexcept(std::pointer_traits<Ptr>::to_address(ptr)))
    -> decltype(std::pointer_traits<Ptr>::to_address(ptr))
{
    return std::pointer_traits<Ptr>::to_address(ptr);
}

template<typename T>
constexpr T *impl(adl_tag, T *ptr, p<2>) noexcept
{
    static_assert(!std::is_function_v<T>, "STL prohibits function pointers in `to_address`");
    return ptr;
}

} // namespace detail_to_address

export namespace grace::memory {

// like `std::to_address` but is SFINAE-friendly
[[nodiscard]] constexpr auto to_address(const auto &ptr)
    noexcept(noexcept(impl(detail_to_address::adl_tag{}, ptr, detail_to_address::p<2>{})))
    -> decltype(impl(detail_to_address::adl_tag{}, ptr, detail_to_address::p<2>{}))
{
    return impl(detail_to_address::adl_tag{}, ptr, detail_to_address::p<2>{});
}

} // namespace grace::memory

// -------------------------------------------------------------------------------------------------

namespace detail_to_address_arr {

template<typename Ptr>
constexpr auto impl(const Ptr &ptr, grace::meta::overload_priority<0>)
requires requires(std::size_t i) {
    // overload for `unique_ptr<T[]>` and `shared_ptr<T[]>`.
    // it isn't perfect by any means but i guess that's something?
    { ptr[i] } -> std::same_as<typename std::pointer_traits<Ptr>::element_type &>;
    { ptr.get() } -> std::same_as<typename std::pointer_traits<Ptr>::element_type *>;
}
{
    return ptr.get();
}

template<typename Ptr>
constexpr auto impl(const Ptr &ptr, grace::meta::overload_priority<1>)
    noexcept(noexcept(grace::memory::to_address(ptr)))
    -> decltype(grace::memory::to_address(ptr))
{
    return grace::memory::to_address(ptr);
}

} // namespace detail_to_address_arr

export namespace grace::memory {

// same as `grace::memory::to_address` but supports `{unique,shared}_ptr<T[]>`
[[nodiscard]] constexpr auto to_address_arr(const auto &ptr)
    noexcept(noexcept(detail_to_address_arr::impl(ptr, meta::overload_priority<1>{})))
    -> decltype(detail_to_address_arr::impl(ptr, meta::overload_priority<1>{}))
{
    return detail_to_address_arr::impl(ptr, meta::overload_priority<1>{});
}

} // namespace grace::memory
