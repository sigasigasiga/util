module;

#include <functional>
#include <utility>

export module grace.fn.bind:operators;

import grace.compat;
import grace.fn.op;

namespace grace::fn::bind {

template<typename T>
[[nodiscard]] constexpr auto equal_to(T &&value)
    noexcept(noexcept(std::bind_front(std::equal_to(), std::forward<T>(value))))
    -> decltype(std::bind_front(std::equal_to(), std::forward<T>(value)))
{
    return std::bind_front(std::equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto not_equal_to(T &&value)
    noexcept(noexcept(std::bind_front(std::not_equal_to(), std::forward<T>(value))))
    -> decltype(std::bind_front(std::not_equal_to(), std::forward<T>(value)))
{
    return std::bind_front(std::not_equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename Container>
[[nodiscard]] constexpr auto index_in(Container &&container)
    noexcept(noexcept(std::bind_front(op::subscript(), std::forward<Container>(container))))
    -> decltype(std::bind_front(op::subscript(), std::forward<Container>(container)))
{
    return std::bind_front(op::subscript(), std::forward<Container>(container));
}

// -------------------------------------------------------------------------------------------------

template<typename Idx>
[[nodiscard]] constexpr auto index_with(Idx &&idx)
    noexcept(noexcept(compat::bind_back(op::subscript(), std::forward<Idx>(idx))))
    -> decltype(compat::bind_back(op::subscript(), std::forward<Idx>(idx)))
{
    return compat::bind_back(op::subscript(), std::forward<Idx>(idx));
}

} // namespace grace::fn::bind
