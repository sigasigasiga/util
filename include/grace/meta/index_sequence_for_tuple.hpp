#pragma once

#include <tuple>
#include <type_traits>

namespace grace::meta {

// Easy and SFINAE-friendly way to make an index sequence
template<
    typename FwdFnTuple,
    typename FnTuple = std::remove_reference_t<FwdFnTuple>,
    std::size_t Size = std::tuple_size<FnTuple>::value>
[[nodiscard]] constexpr std::make_index_sequence<Size> index_sequence_for_tuple() noexcept
{
    return {};
}

} // namespace grace::meta
