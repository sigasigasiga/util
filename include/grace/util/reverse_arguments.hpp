#pragma once

#include <functional>
#include <utility>

namespace grace::util {

namespace detail_reverse_arguments {

template<typename F, std::size_t... Is, typename... Args, std::size_t Last = sizeof...(Is) - 1>
constexpr auto impl(std::index_sequence<Is...>, F &&fn, Args &&...args)
    noexcept(noexcept(std::invoke(
        std::forward<F>(fn),
        std::forward<Args...[Last - Is]>(args...[Last - Is])...
    ))) //
    -> decltype(std::invoke(
        std::forward<F>(fn),
        std::forward<Args...[Last - Is]>(args...[Last - Is])...
    ))
{
    return std::invoke(
        std::forward<F>(fn),
        std::forward<Args...[Last - Is]>(args...[Last - Is])...
    );
}

} // namespace detail_reverse_arguments

template<typename F, typename... Args>
constexpr auto reverse_arguments(F &&fn, Args &&...args) //
    noexcept(noexcept(detail_reverse_arguments::impl(
        std::index_sequence_for<Args...>{},
        std::forward<F>(fn),
        std::forward<Args>(args)...
    ))) //
    -> decltype(detail_reverse_arguments::impl(
        std::index_sequence_for<Args...>{},
        std::forward<F>(fn),
        std::forward<Args>(args)...
    ))
{
    return detail_reverse_arguments::impl(
        std::index_sequence_for<Args...>{},
        std::forward<F>(fn),
        std::forward<Args>(args)...
    );
}

} // namespace grace::util
