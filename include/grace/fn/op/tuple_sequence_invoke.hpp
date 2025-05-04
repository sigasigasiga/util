#pragma once

#include <grace/util/tuple_sequence_invoke.hpp>

namespace grace::fn::op {

class [[nodiscard]] tuple_sequence_invoke
{
public:
    template<typename FnTuple, typename ...Args>
    static constexpr auto operator()(FnTuple &&fn_tuple, const Args &...args)
        noexcept(noexcept(util::tuple_sequence_invoke(std::forward<FnTuple>(fn_tuple), args...)))
        -> decltype(util::tuple_sequence_invoke(std::forward<FnTuple>(fn_tuple), args...))
    {
        return util::tuple_sequence_invoke(std::forward<FnTuple>(fn_tuple), args...);
    }
};

} // namespace grace::fn::op
