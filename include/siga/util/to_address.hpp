#pragma once

#include <memory>

namespace siga::util {

// like `std::to_address` but SFINAE-friendly

template<typename T>
[[nodiscard]] constexpr T *to_address(T *ptr) noexcept
{
    static_assert(!std::is_function_v<T>);
    return ptr;
}

template<typename Ptr>
[[nodiscard]] constexpr auto to_address(const Ptr &ptr, ...)
    noexcept(noexcept((to_address)(ptr.operator->()))) //
    -> decltype((to_address)(ptr.operator->()))
{
    // drill down behavior
    return (to_address)(ptr.operator->());
}

// STL doesn't check that the type is a pointer, so do I because the only thing I want is SFINAE
template<typename Ptr>
[[nodiscard]] constexpr auto to_address(const Ptr &ptr, int = 0)
    noexcept(noexcept(std::pointer_traits<Ptr>::to_address(ptr))) //
    -> decltype(std::pointer_traits<Ptr>::to_address(ptr))
{
    return std::pointer_traits<Ptr>::to_address(ptr);
}

// =================================================================================================

// like `siga::util::to_address` but supports `std::{unique,shared}_ptr<T[]>`
template<typename Ptr>
[[nodiscard]] constexpr auto to_address_arr(const Ptr &ptr, int = 0)
    noexcept(noexcept((to_address)(ptr))) //
    -> decltype((to_address)(ptr))
{
    return (to_address)(ptr);
}

template<typename Ptr>
[[nodiscard]] constexpr auto to_address_arr(const Ptr &ptr, ...)
requires requires(std::size_t i) {
    // overload for `unique_ptr<T[]>` and `shared_ptr<T[]>`.
    // it isn't perfect by any means but i guess that's something?
    { ptr[i] } -> std::same_as<typename std::pointer_traits<Ptr>::element_type &>;
    { ptr.get() } -> std::same_as<typename std::pointer_traits<Ptr>::element_type *>;
}
{
    return ptr.get();
}

} // namespace siga::util
