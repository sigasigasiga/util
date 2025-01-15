#pragma once

#include <utility>

#include <siga/meta/tuple.hpp>

namespace siga::fn::op {

template<typename T>
class [[nodiscard]] get_by_type
{
public:
    template<meta::tuple_like Tuple>
    [[nodiscard]] static constexpr decltype(auto) operator()(Tuple &&tup)
        noexcept(noexcept(get<T>(std::forward<Tuple>(tup))))
    {
        return get<T>(std::forward<Tuple>(tup));
    }
};

template<auto V>
class [[nodiscard]] get_by_value
{
public:
    template<meta::tuple_like Tuple>
    [[nodiscard]] static constexpr decltype(auto) operator()(Tuple &&tup)
        noexcept(noexcept(get<V>(std::declval<Tuple>())))
    {
        return get<V>(std::forward<Tuple>(tup));
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

// -------------------------------------------------------------------------------------------------

// same as `get_by_value` but also check if the type satisfies `pair_like`
class [[nodiscard]] get_key
{
public:
    template<meta::pair_like FwdPair>
    [[nodiscard]] static constexpr decltype(auto) operator()(FwdPair &&pair)
        noexcept(noexcept(get<0>(std::forward<FwdPair>(pair))))
    {
        return get<0>(std::forward<FwdPair>(pair));
    }
};

class [[nodiscard]] get_value
{
public:
    template<meta::pair_like FwdPair>
    [[nodiscard]] static constexpr decltype(auto) operator()(FwdPair &&pair)
        noexcept(noexcept(get<1>(std::forward<FwdPair>(pair))))
    {
        return get<1>(std::forward<FwdPair>(pair));
    }
};

} // namespace siga::fn::op
