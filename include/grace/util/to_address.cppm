module;

#include <concepts>
#include <memory>

export module grace.util.to_address;

import grace.util.utility;

template<std::uintmax_t N>
using p = overload_priority<N>;

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

template<typename Ptr>
constexpr auto impl_arr(const Ptr &ptr, overload_priority<0>)
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
constexpr auto impl_arr(const Ptr &ptr, overload_priority<1>)
    noexcept(noexcept((to_address)(ptr)))
    -> decltype((to_address)(ptr))
{
    return (to_address)(ptr);
}

export namespace grace::util {

// like `std::to_address` but is SFINAE-friendly
[[nodiscard]] constexpr auto to_address(const auto &ptr)
    noexcept(noexcept(impl(adl_tag{}, ptr, p<2>{})))
    -> decltype(impl(adl_tag{}, ptr, p<2>{}))
{
    return impl(adl_tag{}, ptr, p<2>{});
}

// same as `grace::util::to_address` but supports `{unique,shared}_ptr<T[]>`
[[nodiscard]] constexpr auto to_address_arr(const auto &ptr)
    noexcept(noexcept(impl_arr(ptr, overload_priority<1>{})))
    -> decltype(impl_arr(ptr, overload_priority<1>{}))
{
    return impl_arr(ptr, overload_priority<1>{});
}

} // namespace grace::util
