#pragma once

#include <siga/meta/decay_copy.hpp>

namespace siga::fn::op {

// unlike `copy_t`, the return value may be move-constructed
// https://en.cppreference.com/w/cpp/standard_library/decay-copy
class [[nodiscard]] decay_copy
{
public:
    template<typename T>
    [[nodiscard]] static constexpr std::decay_t<T> operator()(T &&value)
        noexcept(meta::is_nothrow_decay_copy_constructible_v<T>)
    {
        return value;
    }
};

} // namespace siga::fn::op
