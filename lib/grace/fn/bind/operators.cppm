module;

#include <functional>
#include <utility>

export module grace.fn.bind:operators;

import grace.compat;
import grace.fn.op;
import grace.fn.wrap.args;

namespace operators {

using namespace grace;

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_front_unwrap(F &&f, Args &&...args)
    noexcept(noexcept(std::bind_front(
        fn::wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    )))
    -> decltype(std::bind_front(
        fn::wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    ))
{
    return std::bind_front(
        fn::wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    );
}

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_back_unwrap(F &&f, Args &&...args)
    noexcept(noexcept(compat::bind_back(
        fn::wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    )))
    -> decltype(compat::bind_back(
        fn::wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    ))
{
    return compat::bind_back(
        fn::wrap::args::unwrap_reference(std::forward<F>(f)),
        std::forward<Args>(args)...
    );
}

} // namespace operators

namespace grace::fn::bind {

template<typename T>
[[nodiscard]] constexpr auto equal_to(T &&value)
    noexcept(noexcept(operators::bind_front_unwrap(std::equal_to(), std::forward<T>(value))))
    -> decltype(operators::bind_front_unwrap(std::equal_to(), std::forward<T>(value)))
{
    return operators::bind_front_unwrap(std::equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto not_equal_to(T &&value)
    noexcept(noexcept(operators::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value))))
    -> decltype(operators::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value)))
{
    return operators::bind_front_unwrap(std::not_equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename Container>
[[nodiscard]] constexpr auto index_in(Container &&container)
    noexcept(noexcept(operators::bind_front_unwrap(op::subscript(), std::forward<Container>(container))))
    -> decltype(operators::bind_front_unwrap(op::subscript(), std::forward<Container>(container)))
{
    return operators::bind_front_unwrap(op::subscript(), std::forward<Container>(container));
}

// -------------------------------------------------------------------------------------------------

template<typename Idx>
[[nodiscard]] constexpr auto index_with(Idx &&idx)
    noexcept(noexcept(operators::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx))))
    -> decltype(operators::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx)))
{
    return operators::bind_back_unwrap(op::subscript(), std::forward<Idx>(idx));
}

} // namespace grace::fn::bind
