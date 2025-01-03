#pragma once

#include <functional>
#include <tuple>

#include <siga/meta/tuple.hpp>

namespace siga::util {

// TODO: move to `siga::fn::op`?
class [[nodiscard]] for_each_in_parameter_pack_t
{
public:
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
    template<typename F, meta::tuple_like Tuple>
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
