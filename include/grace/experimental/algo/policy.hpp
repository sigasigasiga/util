#pragma once

#include <iterator>
#include <optional>

// TODO: should this be in `algo`?
namespace grace::algo {

// TODO: move the policies somewhere else
template<std::input_iterator It>
class identity
{
public:
    using result_type = It;

public:
    // TODO: `noexcept`?
    constexpr static result_type from_iterator(It it) { return it; }
    constexpr static result_type from_end(It it) { return it; }
};

template<std::input_iterator It>
class to_pointer
{
public:
    using result_type = std::iterator_traits<It>::pointer;

public:
    constexpr static result_type from_iterator(It it) noexcept { return std::to_address(it); }
    constexpr static result_type from_end(It) noexcept { return nullptr; }
};

template<std::input_iterator It>
class to_optional
{
    constexpr static bool noexc =
        std::is_nothrow_copy_constructible_v<typename std::iterator_traits<It>::value_type>;

public:
    using result_type = std::optional<typename std::iterator_traits<It>::value_type>;

public:
    constexpr static result_type from_iterator(It it) noexcept(noexc)
    {
        return std::make_optional(*it);
    }

    constexpr static result_type from_end(It it) noexcept { return std::nullopt; }
};

template<template<std::input_iterator> typename Policy, typename It>
concept return_policy_for = std::input_iterator<It> && requires(It it) {
    { Policy<It>::from_iterator(it) } -> std::same_as<typename Policy<It>::result_type>;
    { Policy<It>::from_end(it) } -> std::same_as<typename Policy<It>::result_type>;
};

} // namespace grace::algo
