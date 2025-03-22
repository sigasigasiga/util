#pragma once

#include <utility>

namespace siga::fn::op {

class [[nodiscard]] throw_exception
{
public:
    template<typename T>
    [[noreturn]] static constexpr void operator()(T &&value)
    {
        throw std::forward<T>(value);
    }
};

} // namespace siga::fn::op
