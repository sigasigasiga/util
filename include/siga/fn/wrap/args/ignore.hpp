#pragma once

#include <concepts>
#include <functional>

#include <siga/meta/decay_copy.hpp>
#include <siga/util/storage_base.hpp>

namespace siga::fn::wrap::args {

namespace detail_ignore {

template<std::invocable F>
class [[nodiscard]] impl : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    template<typename Self>
    constexpr auto operator()(this Self &&self, auto &&...)
        noexcept(noexcept(std::invoke(util::forward_self<Self, impl>(self).value()))) //
        -> decltype(std::invoke(util::forward_self<Self, impl>(self).value()))
    {
        return std::invoke(util::forward_self<Self, impl>(self).value());
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

} // namespace siga::fn::wrap::args
