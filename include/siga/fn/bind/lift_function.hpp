#pragma once

#include <utility>

namespace sgia::fn::bind {

namespace detail_lift_function {

template<typename T, auto V>
struct impl;

template<typename Ret, typename... Args, bool Noexcept, Ret (*Fn)(Args...) noexcept(Noexcept)>
struct impl<Ret (*)(Args...) noexcept(Noexcept), Fn>
{
    static constexpr Ret operator()(Args &&...args) noexcept(Noexcept)
    {
        return Fn(std::forward<Args>(args)...);
    }
};

} // namespace detail_lift_function

// make a class from a function pointer that doesn't take any space and
// correctly adds it to the overload set
template<auto V>
using lift_function = detail_lift_function::impl<decltype(V), V>;

} // namespace sgia::fn::bind
