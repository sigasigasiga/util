#pragma once

#include <tuple>

namespace siga::meta {

template<typename Tuple, std::size_t I>
concept index_gettable = requires(Tuple tuple) {
    // relations between `tuple_element_t` and `decltype(get<I>(tuple))` are not pretty clear.
    // the biggest problem in combining these two that i've came across is this:
    //
    // std::tuple<int &&> t = /* ... */;
    // { std::get<0>(t) } -> std::same_as<int &>;

    get<I>(tuple);
    typename std::tuple_element<I, Tuple>::type;
};

template<typename T>
concept tuple_like =
    []<std::size_t... Is>(std::index_sequence<Is...>) constexpr {
        // note that `get<Type>(tuple)` is not always available for `std::tuple`
        // and is never available for `std::array` which is also tuple-like
        return (... && index_gettable<T, Is>);
    }(
        // compiler error messages are better if we don't use `std::tuple_size_v`
        std::make_index_sequence<std::tuple_size<T>::value>{}
    );

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
