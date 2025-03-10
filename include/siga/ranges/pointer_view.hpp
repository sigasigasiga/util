#pragma once

#include <memory>
#include <ranges>

#include <siga/util/to_address.hpp>
#include <siga/util/utility.hpp>

namespace siga::ranges {

// TODO: noexcept

// This class allows to create an owning view from a fancy pointer.
//
// It is better then `views::iota(0uz, size) | views::transform(siga::fn::bind::index_in(ptr))`
// because it only stores 2 pointers, while this combined view stores two `std::size_t`s and a ptr
template<typename Ptr, typename Sent>
requires requires(Ptr ptr, Sent sent) { util::to_address(ptr) == sent; }
class [[nodiscard]] pointer_view : public std::ranges::view_interface<pointer_view<Ptr, Sent>>
{
public:
    constexpr pointer_view(Ptr ptr, Sent sent)
        : ptr_{std::move(ptr)}
        , sent_{std::move(sent)}
    {
    }

    constexpr pointer_view(Ptr ptr, std::size_t n)
        : ptr_{std::move(ptr)}
        , sent_{util::to_address(ptr_) + n}
    {
    }

public:
    [[nodiscard]] constexpr auto begin() const { return util::to_address(ptr_); }
    [[nodiscard]] constexpr auto end() const { return sent_; }

private:
    Ptr ptr_;
    Sent sent_;
};

template<typename Ptr>
pointer_view(Ptr, std::size_t)
    -> pointer_view<Ptr, typename std::pointer_traits<Ptr>::element_type *>;

} // namespace siga::ranges
