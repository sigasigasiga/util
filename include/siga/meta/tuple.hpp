#pragma once

#include <tuple>

#include <siga/meta/copy_cvref.hpp>

namespace siga::meta {

// TODO: check that the return type relates to `std::tuple_element_t`
//
// the biggest problem here is references:
// std::tuple<int &&> t = /* ... */;
// { get<0>(t) } -> std::same_as<int &>;
template<typename FwdTuple, std::size_t I>
concept tuple_index_adl_gettable = requires(FwdTuple &&tuple) {
    requires I < std::tuple_size<std::remove_cvref_t<FwdTuple>>::value;
    get<I>(std::forward<FwdTuple>(tuple));
    typename std::tuple_element<I, std::remove_cvref_t<FwdTuple>>::type;
};

template<typename FwdTuple, std::size_t I>
concept tuple_index_member_gettable = requires(FwdTuple &&tuple) {
    requires I < std::tuple_size<std::remove_cvref_t<FwdTuple>>::value;
    std::forward<FwdTuple>(tuple).template get<I>();
    typename std::tuple_element<I, std::remove_cvref_t<FwdTuple>>::type;
};

template<typename T>
concept tuple_like =
    []<std::size_t... Is>(std::index_sequence<Is...>) constexpr {
        // Notes:
        // 1. `get<Type>(tuple)` is not always available for `std::tuple`
        //    and is never available for `std::array` which is also tuple-like
        // 2. We can't access some elements using `t.get<I>()` and some elements using `get<I>(t)`,
        //    the type must support only one of these options for each of the fields.
        //    https://godbolt.org/z/c6TWssG14
        if constexpr((... || tuple_index_member_gettable<T, Is>)) {
            return (... && tuple_index_member_gettable<T, Is>);
        } else {
            return (... && tuple_index_adl_gettable<T, Is>);
        }
    }(
        // compiler error messages are better if we don't use `std::tuple_size_v`
        std::make_index_sequence<std::tuple_size<std::remove_cvref_t<T>>::value>{}
    );

template<typename T>
concept pair_like = tuple_like<T> && std::tuple_size<std::remove_cvref_t<T>>::value == 2;

} // namespace siga::meta
