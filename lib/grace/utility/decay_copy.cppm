module;

#include <type_traits>
#include <utility>

export module grace.utility:decay_copy;

export namespace grace::utility {

// https://en.cppreference.com/w/cpp/standard_library/decay-copy
// works with explicit copy constructors
template<typename T>
[[nodiscard]] constexpr auto decay_copy(T &&value)
    noexcept(noexcept(std::decay_t<T>(std::forward<T>(value))))
    -> decltype(std::decay_t<T>(std::forward<T>(value)))
{
    return std::decay_t<T>(std::forward<T>(value));
}

} // namespace grace::utility
