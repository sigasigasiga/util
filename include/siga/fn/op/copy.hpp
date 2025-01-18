#pragma once

#include <siga/util/utility.hpp>

namespace siga::fn::op {

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

} // namespace siga::fn::op
