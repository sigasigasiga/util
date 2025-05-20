module;

#include <functional>
#include <utility>

export module grace.util.for_each_in_parameter_pack;

export namespace grace::util {

// Notes:
// 1. I don't personally like passing functor by value then returning it also by value,
//    but this way we are consistent with STL's `std::for_each`
// 2. We cast the invoke result to `void` in order to avoid calling overloaded `operator,`
template<typename F, typename... Args>
constexpr auto for_each_in_parameter_pack(F fn, Args &&...args)
    noexcept(noexcept((..., static_cast<void>(std::invoke(fn, std::forward<Args>(args)))), fn))
    -> decltype((..., static_cast<void>(std::invoke(fn, std::forward<Args>(args)))), fn)
{
    return (..., static_cast<void>(std::invoke(fn, std::forward<Args>(args)))), fn;
}

} // namespace grace::util
