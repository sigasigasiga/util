#pragma once

#include <functional>
#include <utility>

namespace siga::fn::op {

class [[nodiscard]] get_reference
{
public:
    template<typename T>
    [[nodiscard]] static constexpr decltype(auto) operator()(T &&value) noexcept
    {
        return std::forward<T>(value);
    }

    template<typename T>
    [[nodiscard]] static constexpr decltype(auto) operator()(std::reference_wrapper<T> ref) noexcept
    {
        return ref.get();
    }
};

} // namespace siga::fn::op
