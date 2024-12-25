#pragma once

#include <concepts>

namespace siga::meta {

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

} // namespace siga::meta
