#pragma once

#include <siga/fn/op/get_reference.hpp>
#include <siga/util/storage_base.hpp>

namespace siga::fn::wrap {

// TODO: make `apply_trait_wrap` and
// implement `unwrap_reference` as `apply_trait_wrap<std::unwrap_reference>`
template<typename F>
class [[nodiscard]] unwrap_reference : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    // clang-format off
    template<typename Self, typename... Args>
    requires requires(Self &&self, Args &&...args) {
        std::invoke(
            forward_self<Self, unwrap_reference>(self).value(),
            op::get_reference()(std::forward<Args>(args))...
        );
    }
    constexpr decltype(auto) operator()(this Self &&self, Args &&...args)
        noexcept(noexcept(std::invoke(
            forward_self<Self, unwrap_reference>(self).value(),
            op::get_reference()(std::forward<Args>(args))...
        )))
    {
        return std::invoke(
            forward_self<Self, unwrap_reference>(self).value(),
            op::get_reference()(std::forward<Args>(args))...
        );
    }
    // clang-format on
};

template<typename F>
unwrap_reference(F) -> unwrap_reference<F>;

} // namespace siga::fn::wrap
