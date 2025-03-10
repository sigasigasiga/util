#pragma once

#include <functional>

#include <siga/util/storage_base.hpp>

namespace siga::fn::bind {

template<typename F>
class [[nodiscard]] lazy_eval : private util::storage_base<F>
{
public:
    using util::storage_base<F>::storage_base;

public:
    // TODO: you have to write it three times
    template<typename Self, typename UF = meta::copy_cvref_t<Self &&, F>>
    [[nodiscard]] constexpr operator std::invoke_result_t<UF>(this Self &&self)
        noexcept(std::is_nothrow_invocable_v<UF>)
    {
        using USelf = meta::copy_cvref_t<Self &&, lazy_eval>;
        return std::invoke(util::private_base_cast<USelf>(self).value());
    }
};

template<typename F>
lazy_eval(F) -> lazy_eval<F>;

} // namespace siga::fn::bind
