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
        // note that `get<Type>(tuple)` is not always available for `std::tuple`
        // and is never available for `std::array` which is also tuple-like
        return (... && (tuple_index_adl_gettable<T, Is> || tuple_index_member_gettable<T, Is>));
    }(
        // compiler error messages are better if we don't use `std::tuple_size_v`
        std::make_index_sequence<std::tuple_size<std::remove_cvref_t<T>>::value>{}
    );

template<typename T>
concept pair_like = tuple_like<T> && std::tuple_size<std::remove_cvref_t<T>>::value == 2;

// -------------------------------------------------------------------------------------------------

template<typename Tuple, std::size_t I>
class tuple_get_type
{
public:
    using type = decltype(get<I>(std::declval<Tuple>()));
};

template<typename Tuple, std::size_t I>
using tuple_get_type_t = tuple_get_type<Tuple, I>::type;

// -------------------------------------------------------------------------------------------------

template<typename F, typename Tuple>
concept applyable = requires(F &&func, Tuple &&tuple) {
    std::apply(std::forward<F>(func), std::forward<Tuple>(tuple));
};

// -------------------------------------------------------------------------------------------------

template<typename F, typename Tuple>
class apply_result
{
public:
    using type = decltype(std::apply(std::declval<F>(), std::declval<Tuple>()));
};

template<typename F, typename Tuple>
using apply_result_t = apply_result<F, Tuple>::type;

// -------------------------------------------------------------------------------------------------

template<typename F, typename Tuple>
class is_nothrow_applyable
{
public:
    constexpr static bool value = noexcept(std::apply(std::declval<F>(), std::declval<Tuple>()));
};

template<typename F, typename Tuple>
inline constexpr bool is_nothrow_applyable_v = is_nothrow_applyable<F, Tuple>::value;

} // namespace siga::meta
