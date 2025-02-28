#pragma once

#include <functional>

#include <siga/experimental/algo/policy.hpp>

namespace siga::algo {

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
// TODO: how to implement `borrowed_iterator_t`?
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
// TODO: how to implement `borrowed_iterator_t`?
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
