#pragma once

#include <concepts>
#include <type_traits>

namespace siga::fn::op {

// TODO: move traits somewhere else?
template<typename T>
concept decay_copy_constructible = std::constructible_from<std::decay_t<T>, T &&>;

template<typename T>
class is_decay_copy_constructible : public std::is_constructible<std::decay_t<T>, T &&>
{};

template<typename T>
constexpr bool is_decay_copy_constructible_v = is_decay_copy_constructible<T>::value;

template<typename T>
class is_trivially_decay_copy_constructible
    : public std::is_trivially_constructible<std::decay_t<T>, T &&>
{};

template<typename T>
constexpr bool is_trivially_decay_copy_constructible_v =
    is_trivially_decay_copy_constructible<T>::value;

template<typename T>
class is_nothrow_decay_copy_constructible
    : public std::is_nothrow_constructible<std::decay_t<T>, T &&>
{};

template<typename T>
constexpr bool is_nothrow_decay_copy_constructible_v =
    is_nothrow_decay_copy_constructible<T>::value;

// unlike `copy_t`, the return value may be move-constructed
// https://en.cppreference.com/w/cpp/standard_library/decay-copy
class [[nodiscard]] decay_copy
{
public:
    template<typename T>
    [[nodiscard]] static constexpr std::decay_t<T> operator()(T &&value)
        noexcept(is_nothrow_decay_copy_constructible_v<T>)
    {
        return value;
    }
};

} // namespace siga::fn::op
