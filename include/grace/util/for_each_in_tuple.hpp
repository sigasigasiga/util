#pragma once

#include <grace/fn/op/for_each_in_parameter_pack.hpp>

namespace grace::util {

template<typename F, typename Tuple>
constexpr auto for_each_in_tuple(F fn, Tuple &&tuple)
    noexcept(noexcept(std::apply(
        std::bind_front(fn::op::for_each_in_parameter_pack(), std::move(fn)),
        std::forward<Tuple>(tuple)
    )))
    -> decltype(std::apply(
        std::bind_front(fn::op::for_each_in_parameter_pack(), std::move(fn)),
        std::forward<Tuple>(tuple)
    ))
{
    return std::apply(
        std::bind_front(fn::op::for_each_in_parameter_pack(), std::move(fn)),
        std::forward<Tuple>(tuple)
    );
}

} // namespace grace::util
