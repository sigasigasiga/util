#pragma once

#include <functional>
#include <utility>

namespace siga::util {

// Notes:
// 1. I don't personally like passing functor by value,
//    but this way we are consistent with STL's `std::for_each`
// 2. We cast the invoke result to `void` in order to avoid calling overloaded `operator,`
template<typename F, typename... Args>
constexpr F for_each_in_parameter_pack(F fn, Args &&...args)
    noexcept(noexcept((..., static_cast<void>(std::invoke(fn, std::forward<Args>(args))))))
{
    (..., static_cast<void>(std::invoke(fn, std::forward<Args>(args))));
    return fn;
}

} // namespace siga::util
