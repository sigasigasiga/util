#pragma once

#include <grace/util/for_each_in_parameter_pack.hpp>

namespace grace::fn::op {

class for_each_in_parameter_pack
{
public:
    template<typename F, typename... Args>
    constexpr auto operator()(F fn, Args &&...args)
        noexcept(noexcept(util::for_each_in_parameter_pack(std::move(fn), std::forward<Args>(args)...)))
        -> decltype(util::for_each_in_parameter_pack(std::move(fn), std::forward<Args>(args)...))
    {
        return util::for_each_in_parameter_pack(std::move(fn), std::forward<Args>(args)...);
    }
};

} // namespace grace::fn::op
