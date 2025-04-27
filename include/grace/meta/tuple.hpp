#pragma once

#include <tuple>

#include <grace/meta/copy_cvref.hpp>

namespace grace::meta {

namespace detail_tuple_like {

template<typename T, std::size_t I, typename FwdTuple>
concept convertible_to_tuple_element = std::convertible_to<
    T,
    typename std::tuple_element<I, std::remove_reference_t<FwdTuple>>::type &&
>;

template<std::size_t I, typename FwdTuple>
concept member_gettable = requires(FwdTuple &&tup) {
    { std::forward<FwdTuple>(tup).template get<I>() } -> convertible_to_tuple_element<I, FwdTuple>;
};

template<std::size_t I, typename FwdTuple>
concept adl_gettable = requires(FwdTuple &&tup) {
    { get<I>(std::forward<FwdTuple>(tup)) } -> convertible_to_tuple_element<I, FwdTuple>;
};

} // namespace detail_tuple_like

template<typename T>
concept tuple_like =
    []<std::size_t... Is>(std::index_sequence<Is...>) constexpr {
        // Notes:
        // 1. `get<Type>(tuple)` is not always available for `std::tuple`
        //    and is never available for `std::array` which is also tuple-like
        // 2. We can't access some elements using `t.get<I>()` and some elements using `get<I>(t)`,
        //    the type must support only one of these options for each of the fields.
        //    https://godbolt.org/z/c6TWssG14
        if constexpr((... || detail_tuple_like::member_gettable<Is, T>)) {
            return (... && detail_tuple_like::member_gettable<Is, T>);
        } else {
            return (... && detail_tuple_like::adl_gettable<Is, T>);
        }
    }(
        // compiler error messages are better if we don't use `std::tuple_size_v`
        std::make_index_sequence<std::tuple_size<std::remove_reference_t<T>>::value>{}
    );

template<typename T>
concept pair_like = tuple_like<T> && std::tuple_size<std::remove_reference_t<T>>::value == 2;

} // namespace grace::meta
