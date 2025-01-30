#pragma once

#include <functional>
#include <utility>

namespace siga::util {

namespace detail_fold_invoke {

// https://stackoverflow.com/a/39996086/10961484
class adl_tag
{};

template<std::size_t I, typename Tup, typename... Args>
constexpr auto impl(adl_tag, std::index_sequence<I>, Tup &&tup, Args &&...args)                  //
    noexcept(noexcept(std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...))) //
    -> decltype(std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...))
{
    return std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...);
}

template<std::size_t I, std::size_t... Is, typename Tup, typename... Args>
constexpr auto impl(adl_tag, std::index_sequence<I, Is...>, Tup &&tup, Args &&...args) //
    noexcept(noexcept(impl(
        adl_tag{},
        std::index_sequence<Is...>{},
        std::forward<Tup>(tup),
        std::invoke(get<I>(std::forward<Tup>(tup)), std::forward<Args>(args)...)
    ))) //
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

} // namespace detail_fold_invoke

template<
    typename Tup,
    typename... Args,
    std::size_t TupSz = std::tuple_size_v<std::remove_reference_t<Tup>>>
constexpr auto tuple_fold_invoke(Tup &&tup, Args &&...args) //
    noexcept(noexcept(detail_fold_invoke::impl(
        detail_fold_invoke::adl_tag{},
        std::make_index_sequence<TupSz>{},
        std::forward<Tup>(tup),
        std::forward<Args>(args)...
    ))) //
    -> decltype(detail_fold_invoke::impl(
        detail_fold_invoke::adl_tag{},
        std::make_index_sequence<TupSz>{},
        std::forward<Tup>(tup),
        std::forward<Args>(args)...
    ))
{
    return detail_fold_invoke::impl(
        detail_fold_invoke::adl_tag{},
        std::make_index_sequence<TupSz>{},
        std::forward<Tup>(tup),
        std::forward<Args>(args)...
    );
}

} // namespace siga::util
