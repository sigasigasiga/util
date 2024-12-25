#pragma once

#include <type_traits>
#include <utility>

namespace siga::fn::op {

class [[nodiscard]] cut_rvalue_ref
{
public:
    template<typename T>
    [[nodiscard]] static constexpr decltype(auto) operator()(T &&value)
        noexcept(std::is_lvalue_reference_v<T> || std::is_nothrow_move_constructible_v<T>)
    {
        // cut_rvalue_ref(0) -> cut_rvalue_ref<int>(int &&);
        // cut_rvalue_ref(x) -> cut_rvalue_ref<int &>(int &);
        //
        // so if the passed value is lvalue, we return the lvalue,
        // and if the passed value is rvalue, we return the value
        return static_cast<T>(std::forward<T>(value));
    }
};

} // namespace siga::fn::op
