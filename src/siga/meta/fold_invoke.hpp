#pragma once

#include <type_traits>
#include <utility>

#include <siga/util/tuple.hpp>

namespace siga::meta {

template<typename FsTuple, typename... InnerArgs>
class is_nothrow_fold_invocable
{
private:
    template<typename IndexSequence, typename... Args>
    class impl;

    template<std::size_t I, typename... Args>
    class impl<std::index_sequence<I>, Args...>
    {
    private:
        using current_fn_t = tuple_get_type_t<FsTuple, I>;

    public:
        static constexpr bool value = std::is_nothrow_invocable_v<current_fn_t, Args...>;
    };

    template<std::size_t I, std::size_t... Is, typename... Args>
    class impl<std::index_sequence<I, Is...>, Args...>
    {
    private:
        using current_fn_t = tuple_get_type_t<FsTuple, I>;
        using current_ret_t = std::invoke_result_t<current_fn_t, Args...>;

        static constexpr bool current = std::is_nothrow_invocable_v<current_fn_t, Args...>;

    public:
        static constexpr bool value =
            current && impl<std::index_sequence<Is...>, current_ret_t>::value;
    };

private:
    static constexpr std::size_t tuple_size = std::tuple_size_v<std::decay_t<FsTuple>>;

public:
    static constexpr bool value = impl<std::make_index_sequence<tuple_size>, InnerArgs...>::value;
};

template<typename FsTuple, typename... Args>
inline constexpr bool is_nothrow_fold_invocable_v =
    is_nothrow_fold_invocable<FsTuple, Args...>::value;

} // namespace siga::meta
