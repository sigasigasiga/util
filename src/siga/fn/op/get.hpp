#pragma once

#include <utility>

namespace siga::fn::op {

template<typename T>
class [[nodiscard]] get_by_type
{
public:
    template<typename Gettable>
    requires requires(Gettable &&gettable) { get<T>(std::forward<Gettable>(gettable)); }
    [[nodiscard]] static constexpr decltype(auto) operator()(Gettable &&gettable)
        noexcept(noexcept(get<T>(std::declval<Gettable>())))
    {
        return get<T>(std::forward<Gettable>(gettable));
    }
};

template<auto V>
class [[nodiscard]] get_by_value
{
public:
    template<typename Gettable>
    requires requires(Gettable &&gettable) { get<V>(std::forward<Gettable>(gettable)); }
    [[nodiscard]] static constexpr decltype(auto) operator()(Gettable &&gettable)
        noexcept(noexcept(get<V>(std::declval<Gettable>())))
    {
        return get<V>(std::forward<Gettable>(gettable));
    }
};

template<typename T>
constexpr get_by_type<T> make_get() noexcept
{
    return {};
}

template<auto V>
constexpr get_by_value<V> make_get() noexcept
{
    return {};
}

} // namespace siga::fn::op
