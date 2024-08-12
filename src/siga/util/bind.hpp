#pragma once

#include "siga/util/functional.hpp"
#include "siga/util/utility.hpp"

namespace siga::util {

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_front_unwrap(F &&f, Args &&...args) //
    noexcept(
        is_nothrow_decay_copy_constructible_v<F> &&
        (... && is_nothrow_decay_copy_constructible_v<Args>)
    )
{
    return std::bind_front(get_reference_wrap(std::forward<F>(f)), std::forward<Args>(args)...);
}

#if 0
template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_back_unwrap(F &&f, Args &&...args) //
    noexcept(
        is_nothrow_decay_copy_constructible_v<F> &&
        (... && is_nothrow_decay_copy_constructible_v<Args>)
    )
{
    // TODO: use `std::bind_back` when llvm 19
    return ranges::bind_back(get_reference_wrap(std::forward<F>(f)), std::forward<Args>(args)...);
}
#endif

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto equal_to(T &&value)
    noexcept(noexcept(bind_front_unwrap(std::equal_to(), std::forward<T>(value))))
{
    return bind_front_unwrap(std::equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto not_equal_to(T &&value)
    noexcept(noexcept(bind_front_unwrap(std::not_equal_to(), std::forward<T>(value))))
{
    return bind_front_unwrap(std::not_equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename Container>
[[nodiscard]] constexpr auto index_in(Container &&container)
    noexcept(noexcept(bind_front_unwrap(subscript(), std::forward<Container>(container))))
{
    return bind_front_unwrap(subscript(), std::forward<Container>(container));
}

// -------------------------------------------------------------------------------------------------

#if 0
template<typename Idx>
[[nodiscard]] constexpr auto index_with(Idx &&idx)
    noexcept(noexcept(bind_back_unwrap(subscript(), std::forward<Idx>(idx))))
{
    return bind_back_unwrap(subscript(), std::forward<Idx>(idx));
}
#endif

} // namespace siga::util
