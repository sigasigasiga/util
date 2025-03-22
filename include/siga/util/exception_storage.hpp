#pragma once

#include <exception>
#include <type_traits>

namespace siga::util {

// like `std::exception_ptr` but:
// 1. has value semantics
// 2. cannot be null
// 3. stores exceptions that are convertible to `ExBase` by pointer (`ExBase` may be `void`)
template<typename ExBase>
class [[nodiscard]] exception_storage
{
    static_assert(
        std::is_same_v<ExBase, std::remove_cvref_t<ExBase>>,
        "`ExBase` must be cvref-unqualified"
    );

public:
    template<typename FwdEx = ExBase, typename Ex = std::remove_cvref_t<FwdEx>>
    requires std::is_convertible_v<Ex *, ExBase *> && //
             (!std::is_same_v<Ex, exception_storage>)
    explicit exception_storage(FwdEx &&fwd_ex)
        : ep_{std::make_exception_ptr(std::forward<FwdEx>(fwd_ex))}
    {
    }

    template<typename ExDerived>
    requires std::is_convertible_v<ExDerived *, ExBase *> && //
             (!std::is_same_v<ExDerived, ExBase>)
    exception_storage(const exception_storage<ExDerived> &rhs)
        : ep_{rhs.get_exception_ptr()}
    {
    }

public:
    [[noreturn]] void throw_exception() const { std::rethrow_exception(ep_); }
    [[nodiscard]] std::exception_ptr get_exception_ptr() const { return ep_; }

private:
    std::exception_ptr ep_;
};

template<typename Ex>
exception_storage(Ex) -> exception_storage<Ex>;

} // namespace siga::util
