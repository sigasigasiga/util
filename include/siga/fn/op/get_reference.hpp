#pragma once

#include <siga/util/utility.hpp>

namespace siga::fn::op {

class [[nodiscard]] get_reference
{
public:
    template<typename T>
    [[nodiscard]] static constexpr auto operator()(T &&value)
        noexcept(noexcept(util::get_reference(std::forward<T>(value)))) //
        -> decltype(util::get_reference(std::forward<T>(value)))
    {
        return util::get_reference(std::forward<T>(value));
    }
};

} // namespace siga::fn::op
