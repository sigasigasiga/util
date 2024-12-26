#pragma once

namespace siga::util {

template<typename T>
class [[nodiscard]] type_tag
{
public:
    using type = T;
};

template<auto V>
class [[nodiscard]] value_tag
{
public:
    constexpr static auto value = V;
};

template<typename T>
constexpr type_tag<T> make_tag() noexcept
{
    return {};
}

template<auto V>
constexpr value_tag<V> make_tag() noexcept
{
    return {};
}

} // namespace siga::util
