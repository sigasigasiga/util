#pragma once

#include <utility>

namespace grace::fn::op {

class [[nodiscard]] throw_exception
{
public:
    template<typename T>
    [[noreturn]] static constexpr void operator()(T &&value)
    {
        throw std::forward<T>(value);
    }
};

} // namespace grace::fn::op
