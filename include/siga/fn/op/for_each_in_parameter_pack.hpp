#pragma once

#include <siga/util/for_each_in_parameter_pack.hpp>

namespace siga::fn::op {

class for_each_in_parameter_pack
{
public:
    // clang-format off
    template<typename F, typename... Args>
    constexpr auto operator()(F fn, Args &&...args)
        noexcept(noexcept(util::for_each_in_parameter_pack(std::move(fn), std::forward<Args>(args)...)))
        -> decltype(util::for_each_in_parameter_pack(std::move(fn), std::forward<Args>(args)...))
    {
        return util::for_each_in_parameter_pack(std::move(fn), std::forward<Args>(args)...);
    }
    // clang-format on
};

} // namespace siga::fn::op
