#pragma once

#include <siga/util/utility.hpp>

namespace siga::fn::op {

// unlike `copy_t`, the return value may be move-constructed
// https://en.cppreference.com/w/cpp/standard_library/decay-copy
class [[nodiscard]] decay_copy
{
public:
    template<typename T>
    [[nodiscard]] static constexpr auto operator()(T &&value)
        noexcept(noexcept(util::decay_copy(std::forward<T>(value)))) //
        -> decltype(util::decay_copy(std::forward<T>(value)))
    {
        return util::decay_copy(std::forward<T>(value));
    }
};

} // namespace siga::fn::op
