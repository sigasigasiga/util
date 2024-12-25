#pragma once

#include <siga/fn/op/decay_copy.hpp>
#include <siga/fn/op/subscript.hpp>
#include <siga/fn/wrap/unwrap_reference.hpp>
#include <siga/compat/bind_back.hpp>

namespace siga::fn::bind {

namespace detail {

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_front_unwrap(F &&f, Args &&...args) //
    noexcept(
        meta::is_nothrow_decay_copy_constructible_v<F> &&
        (... && meta::is_nothrow_decay_copy_constructible_v<Args>)
    )
{
    return std::bind_front(wrap::unwrap_reference(std::forward<F>(f)), std::forward<Args>(args)...);
}

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_back_unwrap(F &&f, Args &&...args) //
    noexcept(
        meta::is_nothrow_decay_copy_constructible_v<F> &&
        (... && meta::is_nothrow_decay_copy_constructible_v<Args>)
    )
{
    // TODO: use `std::bind_back` when llvm 19
    return compat::bind_back(
        wrap::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    );
}

} // namespace detail

template<typename T>
[[nodiscard]] constexpr auto equal_to(T &&value)
    noexcept(noexcept(detail::bind_front_unwrap(std::equal_to(), std::forward<T>(value))))
{
    return detail::bind_front_unwrap(std::equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto not_equal_to(T &&value)
    noexcept(noexcept(detail::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value))))
{
    return detail::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename Container>
[[nodiscard]] constexpr auto index_in(Container &&container
) noexcept(noexcept(detail::bind_front_unwrap(op::subscript(), std::forward<Container>(container))))
{
    return detail::bind_front_unwrap(op::subscript(), std::forward<Container>(container));
}

// -------------------------------------------------------------------------------------------------

template<typename Idx>
[[nodiscard]] constexpr auto index_with(Idx &&idx)
    noexcept(noexcept(detail::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx))))
{
    return detail::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx));
}

} // namespace siga::fn::bind
