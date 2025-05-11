#pragma once

#include <functional>

#include <grace/util/storage_base.hpp>

namespace grace::fn::bind {

template<typename F>
class [[nodiscard]] lazy_eval : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

private:
    template<typename Self, typename USelf = meta::copy_cvref_t<Self &&, lazy_eval>>
    static auto impl(Self &&self)
        noexcept(noexcept(std::invoke(util::private_base_cast<USelf>(self).value())))
        -> decltype(std::invoke(util::private_base_cast<USelf>(self).value()))
    {
        return std::invoke(util::private_base_cast<USelf>(self).value());
    }

public:
    template<typename Self>
    [[nodiscard]] constexpr operator decltype((impl)(std::declval<Self>()))(this Self &&self)
        noexcept(noexcept((impl)(std::forward<Self>(self))))
    {
        return (impl)(std::forward<Self>(self));
    }

};

template<typename F>
lazy_eval(F) -> lazy_eval<F>;

} // namespace grace::fn::bind
