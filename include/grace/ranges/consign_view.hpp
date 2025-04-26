#pragma once

#include <ranges>

#include <grace/util/scoped.hpp>
#include <grace/util/utility.hpp>

namespace grace::ranges {

// similar to `boost::asio::consign`.
// originally i planned to make `pointer_view` which'd accept a fancy ptr and a sentinel for it
// then the `begin` would be implemented as `std::to_pointer(fancy_ptr_)` and `end` is just `sent_`
// but `std::to_pointer` is not available for `std::unique_ptr<T[]>`, so i came up with this
template<std::ranges::view View, typename Value>
class consign_view : public std::ranges::view_interface<consign_view<View, Value>>,
                     // standard requires that views must copy and move construction must be O(1)
                     // we cannot guarantee that copying of `Value` is O(1), so we have disable it
                     private util::move_only
{
private:
    View view_;
    Value value_;

public:
    constexpr consign_view(View view, Value value)
        : view_{std::move(view)}
        , value_{std::move(value)}
    {
    }

public:
    // clang-format off
    template<typename Self, typename USelf = meta::copy_cvref_t<Self &&, consign_view>>
    constexpr auto base(this Self &&self)
        noexcept(noexcept(View(std::forward<Self>(self).view_)))
        -> decltype(View(std::forward<Self>(self).view_))
    {
        return View(util::private_base_cast<USelf>(self).view_);
    }

    constexpr auto begin() const
        noexcept(noexcept(std::ranges::begin(this->view_)))
        -> decltype(std::ranges::begin(this->view_))
    {
        return std::ranges::begin(this->view_);
    }

    constexpr auto end() const
        noexcept(noexcept(std::ranges::end(this->view_)))
        -> decltype(std::ranges::end(this->view_))
    {
        return std::ranges::end(this->view_);
    }

    constexpr auto size() const
        noexcept(noexcept(std::ranges::size(this->view_)))
        -> decltype(std::ranges::size(this->view_))
    {
        return std::ranges::size(this->view_);
    }
    // clang-format on
};

namespace views {

template<typename Value>
class consign_closure : public std::ranges::range_adaptor_closure<consign_closure<Value>>
{
public:
    constexpr consign_closure(Value value)
        : value_{std::move(value)}
    {
    }

public:
    // clang-format off
    template<typename Self, std::ranges::view View>
    constexpr auto operator()(this Self &&self, View &&view)
        noexcept(noexcept(consign_view(std::forward<View>(view), std::forward<Self>(self).value_)))
        -> decltype(consign_view(std::forward<View>(view), std::forward<Self>(self).value_))
    {
        return consign_view(std::forward<View>(view), std::forward<Self>(self).value_);
    }
    // clang-format on

private:
    Value value_;
};

constexpr auto consign(auto value)                        //
    noexcept(noexcept(consign_closure(std::move(value)))) //
    -> decltype(consign_closure(std::move(value)))
{
    return consign_closure(std::move(value));
}

} // namespace views

} // namespace grace::ranges
