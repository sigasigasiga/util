#pragma once

#include <functional>

#include <grace/meta/concepts.hpp>

namespace grace::fn::bind {

// TODO: come up with a better name? because we return reference, not value
template<typename T>
[[nodiscard]] constexpr auto return_value(T &&value)
    noexcept(noexcept(std::bind_front(std::identity{}, std::forward<T>(value)))) //
    -> decltype(std::bind_front(std::identity{}, std::forward<T>(value)))
{
    return std::bind_front(std::identity{}, std::forward<T>(value));
}

template<meta::conceptify<std::is_void> T = void>
[[nodiscard]] constexpr auto return_value()
{
    return [] static constexpr noexcept {
    };
}

} // namespace grace::fn::bind
