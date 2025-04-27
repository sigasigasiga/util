#pragma once

#include <functional>

#include <grace/util/storage_base.hpp>
#include <grace/util/utility.hpp>

namespace grace::fn::wrap::args {

namespace detail_unwrap_reference {

template<typename F>
class [[nodiscard]] impl : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<typename Self, typename... Args, typename USelf = meta::copy_cvref_t<Self &&, impl>>
    constexpr auto operator()(this Self &&self, Args &&...args)
        noexcept(noexcept(std::invoke(
            util::private_base_cast<USelf>(self).value(),
            util::get_reference(std::forward<Args>(args))...
        )))
        -> decltype(std::invoke(
            util::private_base_cast<USelf>(self).value(),
            util::get_reference(std::forward<Args>(args))...
        ))
    {
        return std::invoke(
            util::private_base_cast<USelf>(self).value(),
            util::get_reference(std::forward<Args>(args))...
        );
    }
};

template<typename F>
impl(F) -> impl<F>;

} // namespace detail_unwrap_reference

template<typename F>
[[nodiscard]] constexpr auto unwrap_reference(F &&fn)
    noexcept(noexcept(detail_unwrap_reference::impl(std::forward<F>(fn))))
    -> decltype(detail_unwrap_reference::impl(std::forward<F>(fn)))
{
    return detail_unwrap_reference::impl(std::forward<F>(fn));
}

} // namespace grace::fn::wrap::args
