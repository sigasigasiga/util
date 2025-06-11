module;

#include <functional>

export module grace.fn.invoke:sequence;

import grace.tuple;

namespace {

template<typename FnTuple, std::size_t... Is, typename... Args>
auto impl(std::index_sequence<Is...>, FnTuple &&fn_tuple, const Args &...args)
    noexcept(noexcept((..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...)))))
    -> decltype((..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...))))
{
    return (..., void(std::invoke(get<Is>(std::forward<FnTuple>(fn_tuple)), args...)));
}

} // anonymous namespace

export namespace grace::fn::invoke {

// TODO: I'm not sure if allowing mutable references is a good idea, so it is `const` for now
template<typename FnTuple, typename... Args>
auto sequence(FnTuple &&fn_tuple, const Args &...args)
    noexcept(noexcept(impl(
        tuple::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    )))
    -> decltype(impl(
        tuple::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    ))
{
    return impl(
        tuple::index_sequence_for_tuple<FnTuple>(),
        std::forward<FnTuple>(fn_tuple),
        args...
    );
}

} // namespace grace::fn::invoke
