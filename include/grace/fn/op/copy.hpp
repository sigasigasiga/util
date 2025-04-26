#pragma once

#include <grace/util/utility.hpp>

namespace grace::fn::op {

class [[nodiscard]] copy
{
public:
    template<typename T>
    [[nodiscard]] static constexpr auto operator()(const T &v) //
        noexcept(noexcept(util::copy(v)))                      //
        -> decltype(util::copy(v))
    {
        return util::copy(v);
    }
};

} // namespace grace::fn::op
