module;

#include <functional>

export module grace.fn.wrap.args:ignore;

import grace.type_traits;
import grace.utility;

namespace {

using namespace grace;

template<typename F>
class [[nodiscard]] impl : private utility::storage_base<F>
{
public:
    using utility::storage_base<F>::storage_base;

public:
    template<typename Self, typename USelf = type_traits::copy_cvref_t<Self &&, impl>>
    constexpr auto operator()(this Self &&self, auto &&...)
        noexcept(noexcept(std::invoke(utility::private_base_cast<USelf>(self).value())))
        -> decltype(std::invoke(utility::private_base_cast<USelf>(self).value()))
    {
        return std::invoke(utility::private_base_cast<USelf>(self).value());
    }
};

template<typename T>
impl(T) -> impl<T>;

} // anonymous namespace

export namespace grace::fn::wrap::args {

template<std::invocable F>
[[nodiscard]] constexpr auto ignore(F &&fn)
    noexcept(noexcept(impl(std::forward<F>(fn))))
    -> decltype(impl(std::forward<F>(fn)))
{
    return impl(std::forward<F>(fn));
}

} // namespace grace::fn::wrap::args
