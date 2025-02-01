#pragma once

#include <tuple>

#include <siga/meta/copy_cvref.hpp>

namespace siga::meta {

namespace detail_tuple_like {

template<typename FwdTuple, std::size_t I>
concept member_gettable = requires(FwdTuple &&tuple) {
    {
        std::forward<FwdTuple>(tuple).template get<I>()
    } -> std::convertible_to<std::tuple_element_t<I, std::remove_reference_t<FwdTuple>> &&>;
};

template<typename FwdTuple, std::size_t I>
concept adl_gettable = requires(FwdTuple &&tuple) {
    {
        get<I>(std::forward<FwdTuple>(tuple))
    } -> std::convertible_to<std::tuple_element_t<I, std::remove_reference_t<FwdTuple>> &&>;
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
        if constexpr((... || detail_tuple_like::member_gettable<T, Is>)) {
            return (... && detail_tuple_like::member_gettable<T, Is>);
        } else {
            return (... && detail_tuple_like::adl_gettable<T, Is>);
        }
    }(
        // compiler error messages are better if we don't use `std::tuple_size_v`
        std::make_index_sequence<std::tuple_size<std::remove_reference_t<T>>::value>{}
    );

template<typename T>
concept pair_like = tuple_like<T> && std::tuple_size<std::remove_reference_t<T>>::value == 2;

} // namespace siga::meta
