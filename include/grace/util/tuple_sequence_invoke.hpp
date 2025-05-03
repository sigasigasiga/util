#pragma once

#include <functional>

#include <grace/meta/index_sequence_for_tuple.hpp>

namespace grace::util {

namespace detail_tuple_seqence_invoke {

template<typename FnTuple, std::size_t... Is, typename... Args>
auto impl(std::index_sequence<Is...>, FnTuple &&fn_tuple, const Args &...args)
    noexcept(noexcept((..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...)))))
    -> decltype((..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...))))
{
    return (..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...)));
}

} // namespace detail_tuple_seqence_invoke

// TODO: I'm not sure if allowing mutable references is a good idea, so it is `const` for now
template<typename FnTuple, typename... Args>
auto tuple_sequence_invoke(FnTuple &&fn_tuple, const Args &...args)
    noexcept(noexcept(detail_tuple_seqence_invoke::impl(
        meta::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    )))
    -> decltype(detail_tuple_seqence_invoke::impl(
        meta::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    ))
{
    return detail_tuple_seqence_invoke::impl(
        meta::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    );
}

} // namespace grace::util
