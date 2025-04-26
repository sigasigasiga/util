#pragma once

#include <concepts>
#include <functional>

#include <grace/util/storage_base.hpp>

namespace grace::fn::wrap::args {

namespace detail_ignore {

template<std::invocable F>
class [[nodiscard]] impl : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<typename Self, typename USelf = meta::copy_cvref_t<Self &&, impl>>
    constexpr auto operator()(this Self &&self, auto &&...)
        noexcept(noexcept(std::invoke(util::private_base_cast<USelf>(self).value()))) //
        -> decltype(std::invoke(util::private_base_cast<USelf>(self).value()))
    {
        return std::invoke(util::private_base_cast<USelf>(self).value());
    }
};

template<typename T>
impl(T) -> impl<T>;

} // namespace detail_ignore

template<std::invocable F>
[[nodiscard]] constexpr auto ignore(F &&fn)
    noexcept(noexcept(detail_ignore::impl(std::forward<F>(fn)))) //
    -> decltype(detail_ignore::impl(std::forward<F>(fn)))
{
    return detail_ignore::impl(std::forward<F>(fn));
}

} // namespace grace::fn::wrap::args
