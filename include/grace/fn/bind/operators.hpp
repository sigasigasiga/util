#pragma once

#include <grace/compat/bind_back.hpp>
#include <grace/fn/op/subscript.hpp>
#include <grace/fn/wrap/args/unwrap_reference.hpp>

namespace grace::fn::bind {

namespace detail {

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_front_unwrap(F &&f, Args &&...args)
    noexcept(noexcept(std::bind_front(
        wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    )))
    -> decltype(std::bind_front(
        wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    ))
{
    return std::bind_front(
        wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    );
}

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_back_unwrap(F &&f, Args &&...args)
    noexcept(noexcept(compat::bind_back(
        wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    )))
    -> decltype(compat::bind_back(
        wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    ))
{
    return compat::bind_back(
        wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    );
}

} // namespace detail

template<typename T>
[[nodiscard]] constexpr auto equal_to(T &&value)
    noexcept(noexcept(detail::bind_front_unwrap(std::equal_to(), std::forward<T>(value))))
    -> decltype(detail::bind_front_unwrap(std::equal_to(), std::forward<T>(value)))
{
    return detail::bind_front_unwrap(std::equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto not_equal_to(T &&value)
    noexcept(noexcept(detail::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value))))
    -> decltype(detail::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value)))
{
    return detail::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename Container>
[[nodiscard]] constexpr auto index_in(Container &&container)
    noexcept(noexcept(detail::bind_front_unwrap(op::subscript(), std::forward<Container>(container))))
    -> decltype(detail::bind_front_unwrap(op::subscript(), std::forward<Container>(container)))
{
    return detail::bind_front_unwrap(op::subscript(), std::forward<Container>(container));
}

// -------------------------------------------------------------------------------------------------

template<typename Idx>
[[nodiscard]] constexpr auto index_with(Idx &&idx)
    noexcept(noexcept(detail::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx))))
    -> decltype(detail::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx)))
{
    return detail::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx));
}

} // namespace grace::fn::bind
