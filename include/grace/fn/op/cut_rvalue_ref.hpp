#pragma once

#include <grace/util/utility.hpp>

namespace grace::fn::op {

class [[nodiscard]] cut_rvalue_ref
{
public:
    template<typename T>
    [[nodiscard]] static constexpr auto operator()(T &&value)
        noexcept(noexcept(util::cut_rvalue_ref(std::forward<T>(value))))
        -> decltype(util::cut_rvalue_ref(std::forward<T>(value)))
    {
        return util::cut_rvalue_ref(std::forward<T>(value));
    }
};

} // namespace grace::fn::op
