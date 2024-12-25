#pragma once

#include <functional>

#include <siga/util/meta.hpp>

namespace siga::fn::bind {

// TODO: come up with a better name? because we return reference, not value
template<typename T>
[[nodiscard]] constexpr auto return_value(T &&value)
    noexcept(noexcept(std::bind_front(std::identity{}, std::forward<T>(value))))
{
    return std::bind_front(std::identity{}, std::forward<T>(value));
}

template<util::conceptify<std::is_void> T = void>
[[nodiscard]] constexpr auto return_value()
{
    return [] noexcept {
    };
}

} // namespace siga::fn::bind
