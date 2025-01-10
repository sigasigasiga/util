#pragma once

#include <functional>

#include <siga/algo/policy.hpp>

namespace siga::algo {

// clang-format off
template<
    template<std::input_iterator> typename Pol = identity,
    typename I,
    typename S,
    typename Pred,
    typename Proj = std::identity>
requires
    return_policy_for<Pol, I> &&
    std::input_iterator<I> &&
    std::sentinel_for<S, I> &&
    std::indirect_unary_predicate<Pred, std::projected<I, Proj>>
[[nodiscard]] constexpr auto find_if(I first, S last, Pred pred, Proj proj = {})
{
    using policy_t = Pol<I>;

    for(; first != last; ++first) {
        if(std::invoke(pred, std::invoke(proj, *first))) {
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
    typename Pred,
    typename Proj = std::identity,
    typename I = std::ranges::iterator_t<R>>
requires
    return_policy_for<Pol, I> &&
    std::ranges::range<R> &&
    std::indirect_unary_predicate<Pred, std::projected<I, Proj>>
[[nodiscard]] constexpr auto find_if(R &&range, Pred pred, Proj proj = {})
{
    return (find_if<Pol>)(
        std::ranges::begin(range),
        std::ranges::end(range),
        std::ref(pred),
        std::ref(proj)
    );
}
// clang-format on

} // namespace siga::algo
