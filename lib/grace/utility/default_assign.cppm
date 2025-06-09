module;

#include <memory>
#include <type_traits>
#include <utility>

export module grace.utility:default_assign;

export namespace grace::utility {

// Generic way to implement assignment for any class that has
// 1. `To(FwdFrom &&)` construcotr
// 2. `.swap` method
//
// NB: I don't really like the idea of returning an lvalue reference
//     but I don't like inconsistencies with the standard even more
template<typename FwdTo, typename FwdFrom, typename To = std::remove_cvref_t<FwdTo>>
FwdTo &default_assign(FwdTo &&to, FwdFrom &&from)
    noexcept(noexcept(To(std::forward<FwdFrom>(from)).swap(to)))
    requires requires { To(std::forward<FwdFrom>(from)).swap(to); }
{
    if(std::addressof(to) != std::addressof(from)) {
        To(std::forward<FwdFrom>(from)).swap(to);
    }

    return to;
}

} // namespace grace::utility
