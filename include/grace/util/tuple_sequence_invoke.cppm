module;

#include <functional>

export module grace.util.tuple_sequence_invoke;

import grace.meta.index_sequence_for_tuple;

template<typename FnTuple, std::size_t... Is, typename... Args>
auto impl(std::index_sequence<Is...>, FnTuple &&fn_tuple, const Args &...args)
    noexcept(noexcept((..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...)))))
    -> decltype((..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...))))
{
    return (..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...)));
}

export namespace grace::util {

// TODO: I'm not sure if allowing mutable references is a good idea, so it is `const` for now
template<typename FnTuple, typename... Args>
auto tuple_sequence_invoke(FnTuple &&fn_tuple, const Args &...args)
    noexcept(noexcept(impl(
        meta::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    )))
    -> decltype(impl(
        meta::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    ))
{
    return impl(
        meta::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    );
}

} // namespace grace::util
