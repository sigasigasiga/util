module;

#include <functional>
#include <utility>

export module grace.fn.invoke:reverse_arguments;

namespace reverse_arguments {

template<typename F, std::size_t... Is, typename... Args, std::size_t Last = sizeof...(Is) - 1>
constexpr auto impl(std::index_sequence<Is...>, F &&fn, Args &&...args)
    noexcept(noexcept(std::invoke(
        std::forward<F>(fn),
        std::forward<Args...[Last - Is]>(args...[Last - Is])...
    )))
    -> decltype(std::invoke(
        std::forward<F>(fn),
        std::forward<Args...[Last - Is]>(args...[Last - Is])...
    ))
{
    return std::invoke(
        std::forward<F>(fn),
        std::forward<Args...[Last - Is]>(args...[Last - Is])...
    );
}

} // namespace revernse_arguments

export namespace grace::fn::invoke {

template<typename F, typename... Args>
constexpr auto reverse_arguments(F &&fn, Args &&...args)
    noexcept(noexcept(reverse_arguments::impl(
        std::index_sequence_for<Args...>{},
        std::forward<F>(fn),
        std::forward<Args>(args)...
    )))
    -> decltype(reverse_arguments::impl(
        std::index_sequence_for<Args...>{},
        std::forward<F>(fn),
        std::forward<Args>(args)...
    ))
{
    return reverse_arguments::impl(
        std::index_sequence_for<Args...>{},
        std::forward<F>(fn),
        std::forward<Args>(args)...
    );
}

} // namespace grace::fn::invoke
