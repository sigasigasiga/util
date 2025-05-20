module;

#include <functional>
#include <utility>

export module grace.util.tuple_fold_invoke;

import grace.meta.index_sequence_for_tuple;

// https://stackoverflow.com/a/39996086/10961484
class adl_tag
{};

template<std::size_t I, typename Tup, typename... Args>
constexpr auto impl(adl_tag, std::index_sequence<I>, Tup &&tup, Args &&...args)
    noexcept(noexcept(std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...)))
    -> decltype(std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...))
{
    return std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...);
}

template<std::size_t I, std::size_t... Is, typename Tup, typename... Args>
constexpr auto impl(adl_tag, std::index_sequence<I, Is...>, Tup &&tup, Args &&...args)
    noexcept(noexcept(impl(
        adl_tag{},
        std::index_sequence<Is...>{},
        std::forward<Tup>(tup),
        std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...)
    )))
    -> decltype(impl(
        adl_tag{},
        std::index_sequence<Is...>{},
        std::forward<Tup>(tup),
        std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...)
    ))
{
    return impl(
        adl_tag{},
        std::index_sequence<Is...>{},
        std::forward<Tup>(tup),
        std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...)
    );
}

export namespace grace::util {

template<typename Tup, typename ...Args>
constexpr auto tuple_fold_invoke(Tup &&tup, Args &&...args)
    noexcept(noexcept(impl(
        adl_tag{},
        meta::index_sequence_for_tuple<Tup>(),
        std::forward<Tup>(tup),
        std::forward<Args>(args)...
    )))
    -> decltype(impl(
        adl_tag{},
        meta::index_sequence_for_tuple<Tup>(),
        std::forward<Tup>(tup),
        std::forward<Args>(args)...
    ))
{
    return impl(
        adl_tag{},
        meta::index_sequence_for_tuple<Tup>(),
        std::forward<Tup>(tup),
        std::forward<Args>(args)...
    );
}

} // namespace grace::util
