#pragma once

#include <functional>
#include <utility>
#include <version>

namespace grace::compat {

namespace detail {

#ifdef __cpp_lib_bind_back

template<typename... Args>
constexpr auto my_bind_back(Args &&...args)
    noexcept(noexcept(std::bind_back(std::forward<Args>(args)...))) //
    -> decltype(std::bind_back(std::forward<Args>(args)...))
{
    return std::bind_back(std::forward<Args>(args)...);
}

#else // __cpp_lib_bind_back

template<typename F, typename... Args>
constexpr auto my_bind_back(F &&fn, Args &&...args)
    noexcept(noexcept([_ = std::forward<F>(fn), ... _(std::forward<Args>(args))] {}))
{
    // clang-format off
    return
        [
            fn = std::forward<F>(fn),
            ...bound_args(std::forward<Args>(args))
        ]
        <typename Self, typename... Front>
        (
            this Self &&,
            Front &&...front
        )
            noexcept(noexcept(std::invoke(
                std::forward_like<Self>(fn),
                std::forward<Front>(front)...,
                std::forward_like<Self>(bound_args)...
            )))
            -> decltype(std::invoke(
                std::forward_like<Self>(fn),
                std::forward<Front>(front)...,
                std::forward_like<Self>(bound_args)...
            ))
        {
            return std::invoke(
                std::forward_like<Self>(fn),
                std::forward<Front>(front)...,
                std::forward_like<Self>(bound_args)...
            );
        };
    // clang-format on
}

#endif // __cpp_lib_bind_back

} // namespace detail

template<typename... Args>
constexpr auto bind_back(Args &&...args)
{
    return detail::my_bind_back(std::forward<Args>(args)...);
}

} // namespace grace::compat
