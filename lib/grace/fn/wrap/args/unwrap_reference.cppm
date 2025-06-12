module;

#include <functional>

export module grace.fn.wrap.args:unwrap_reference;

import grace.type_traits;
import grace.utility;

namespace unwrap_reference {

using namespace grace;

template<typename F>
class [[nodiscard]] impl : private utility::storage_base<F>
{
public:
    using utility::storage_base<F>::storage_base;

public:
    template<
        typename Self,
        typename... Args,
        typename USelf = type_traits::copy_cvref_t<Self &&, impl>>
    constexpr auto operator()(this Self &&self, Args &&...args)
        noexcept(noexcept(std::invoke(
            utility::private_base_cast<USelf>(self).value(),
            utility::get_reference(std::forward<Args>(args))...
        )))
        -> decltype(std::invoke(
            utility::private_base_cast<USelf>(self).value(),
            utility::get_reference(std::forward<Args>(args))...
        ))
    {
        return std::invoke(
            utility::private_base_cast<USelf>(self).value(),
            utility::get_reference(std::forward<Args>(args))...
        );
    }
};

template<typename F>
impl(F) -> impl<F>;

} // namespace unwrap_reference

export namespace grace::fn::wrap::args {

template<typename F>
[[nodiscard]] constexpr auto unwrap_reference(F &&fn)
    noexcept(noexcept(unwrap_reference::impl(std::forward<F>(fn))))
    -> decltype(unwrap_reference::impl(std::forward<F>(fn)))
{
    return unwrap_reference::impl(std::forward<F>(fn));
}

} // namespace grace::fn::wrap::args
