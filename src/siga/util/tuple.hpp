#pragma once

#include <functional>
#include <tuple>

namespace siga::util {

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

// TODO: move to `siga::fn::op`?
class [[nodiscard]] for_each_in_parameter_pack_t
{
public:
    // well, if `std::for_each` and `std::ranges::for_each` are not marked as `noexcept`,
    // why should i do that here?
    //
    // however, it should probably be written like so:
    // ```
    // noexcept(
    //     (... && std::is_nothrow_invocable_v<F, Args &&>) &&
    //     std::is_nothrow_move_constructible_v<F>
    // )
    // ```
    template<typename F, typename... Args>
    static constexpr F operator()(F func, Args &&...args)
    {
        (..., static_cast<void>(std::invoke(func, std::forward<Args>(args))));
        return func;
    }
};

inline constexpr for_each_in_parameter_pack_t for_each_in_parameter_pack;

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] for_each_in_tuple_t
{
public:
    // see `noexcept` policy for `for_each_in_parameter_pack`
    template<typename F, tuple_like Tuple>
    static constexpr F operator()(F func, Tuple &&tuple)
    {
        return std::apply(
            std::bind_front(for_each_in_parameter_pack, std::move(func)),
            std::forward<Tuple>(tuple)
        );
    }
};

inline constexpr for_each_in_tuple_t for_each_in_tuple;

} // namespace siga::util
