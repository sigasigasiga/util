#pragma once

#include <utility>

namespace siga::fn::op {

template<typename T, bool UseRoundBrackets = true>
class [[nodiscard]] construct
{
public:
    template<typename... Args>
    [[nodiscard]] static constexpr auto operator()(Args &&...args)
        noexcept(noexcept(T(std::forward<Args>(args)...))) //
        -> decltype(T(std::forward<Args>(args)...))
    {
        return T(std::forward<Args>(args)...);
    }
};

template<typename T>
class [[nodiscard]] construct<T, false>
{
public:
    template<typename... Args>
    [[nodiscard]] static constexpr auto operator()(Args &&...args)
        noexcept(noexcept(T{std::forward<Args>(args)...})) //
        -> decltype(T{std::forward<Args>(args)...})
    {
        return T{std::forward<Args>(args)...};
    }
};

template<>
class [[nodiscard]] construct<void>
{
public:
    static constexpr void operator()() noexcept {}
};

} // namespace siga::fn::op
