#pragma once

#include <utility>

#include <siga/meta/fold_invoke.hpp>

namespace siga::fn::op {

class [[nodiscard]] fold_invoke
{
public:
    template<typename FuncTuple, typename... Args>
    static constexpr decltype(auto) operator()(FuncTuple &&funcs, Args &&...args)
        noexcept(meta::is_nothrow_fold_invocable_v<FuncTuple &&, Args &&...>)
    {
        return impl(
            std::make_index_sequence<std::tuple_size_v<std::decay_t<FuncTuple>>>{},
            std::forward<FuncTuple>(funcs),
            std::forward<Args>(args)...
        );
    }

private:
    // clang-format off
    template<typename FsTuple, std::size_t I, std::size_t... Is, typename... Args>
    requires std::invocable<meta::tuple_get_type_t<FsTuple &&, I>, Args &&...>
    static constexpr decltype(auto) impl(
        std::index_sequence<I, Is...>,
        FsTuple &&fs,
        Args &&...args
    ) {
        if constexpr(sizeof...(Is) == 0) {
            return std::invoke(get<I>(std::forward<FsTuple>(fs)), std::forward<Args>(args)...);
        } else {
            return impl(
                std::index_sequence<Is...>{},
                std::forward<FsTuple>(fs),
                std::invoke(get<I>(std::forward<FsTuple>(fs)), std::forward<Args>(args)...)
            );
        }
    }
    // clang-format on
};

} // namespace siga::fn::op
