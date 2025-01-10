#pragma once

#include <functional>
#include <iterator>
#include <ranges>

namespace siga::algo {

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

// clang-format off
template<
    template<std::input_iterator> typename Pol = identity,
    typename I,
    typename S,
    typename T,
    typename Proj = std::identity>
requires
    return_policy_for<Pol, I> &&
    std::input_iterator<I> &&
    std::sentinel_for<S, I> &&
    std::indirect_binary_predicate<std::ranges::equal_to, std::projected<I, Proj>, const T *>
[[nodiscard]] constexpr auto find(I first, S last, const T &needle, Proj proj = {})
{
    using policy_t = Pol<I>;

    for(; first != last; ++first) {
        if(std::invoke(proj, *first) == needle) {
            return policy_t::from_iterator(first);
        }
    }
    return policy_t::from_end(first);
}
// clang-format on

// clang-format off
template<
    template<std::input_iterator> typename Pol = identity,
    typename R,
    typename T,
    typename Proj = std::identity,
    typename I = std::ranges::iterator_t<R>>
requires
    return_policy_for<Pol, I> &&
    std::ranges::range<R> &&
    std::indirect_binary_predicate<std::ranges::equal_to, std::projected<I, Proj>, const T *>
[[nodiscard]] constexpr auto find(R &&range, const T &needle, Proj proj = {})
{
    return (find<Pol>)(std::ranges::begin(range), std::ranges::end(range), needle, std::ref(proj));
}
// clang-format on

// clang-format off
template<
    template<std::input_iterator> typename Pol = identity,
    typename R,
    typename T,
    typename I = std::ranges::iterator_t<R>>
requires
    return_policy_for<Pol, I> &&
    std::ranges::range<R> &&
    requires(R r, T needle) { { r.find(needle) } -> std::input_iterator; }
[[nodiscard]] constexpr auto find(R &&r, const T &needle)
{
    using policy_t = Pol<I>;

    auto it = r.find(needle);
    if(it == r.end()) {
        return policy_t::from_end(it);
    } else {
        return policy_t::from_iterator(it);
    }
}
// clang-format on

} // namespace siga::algo
