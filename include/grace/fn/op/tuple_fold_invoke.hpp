#pragma once

#include <grace/util/tuple_fold_invoke.hpp>

namespace grace::fn::op {

class [[nodiscard]] tuple_fold_invoke
{
public:
    // clang-format off
    template<typename FuncTuple, typename... Args>
    static constexpr auto operator()(FuncTuple &&tup, Args &&...args)
        noexcept(noexcept(util::tuple_fold_invoke(std::forward<FuncTuple>(tup), std::forward<Args>(args)...)))
        -> decltype(util::tuple_fold_invoke( std::forward<FuncTuple>(tup), std::forward<Args>(args)...))
    {
        return util::tuple_fold_invoke(std::forward<FuncTuple>(tup), std::forward<Args>(args)...);
    }
    // clang-format on
};

} // namespace grace::fn::op
