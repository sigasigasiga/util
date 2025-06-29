module;

#include <functional>
#include <utility>

export module grace.utility:objectify;

import :cut_rvalue_ref;

export namespace grace::utility {

template<typename T>
[[nodiscard]] constexpr auto objectify(T &&v)
    noexcept(noexcept((cut_rvalue_ref)(std::forward<T>(v))))
    -> decltype((cut_rvalue_ref)(std::forward<T>(v)))
{
    return (cut_rvalue_ref)(std::forward<T>(v));
}

template<typename T>
[[nodiscard]] constexpr auto objectify(T &v)
    noexcept(noexcept(std::ref(v)))
    -> decltype(std::ref(v))
{
    return std::ref(v);
}

} // namespace grace::utility
