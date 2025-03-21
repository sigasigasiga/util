#pragma once

#include <exception>
#include <type_traits>

namespace siga::util {

// like `std::exception_ptr` but has value semantics and cannot be null
class [[nodiscard]] exception_storage
{
public:
    template<typename Ex>
    requires(!std::is_same_v<std::remove_cvref_t<Ex>, exception_storage>)
    explicit exception_storage(Ex &&ex)
        : ep_{std::make_exception_ptr(std::forward<Ex>(ex))}
    {
    }

public:
    [[noreturn]] void throw_exception() const { std::rethrow_exception(ep_); }

private:
    std::exception_ptr ep_;
};

// -------------------------------------------------------------------------------------------------

// like `exception_storage` but stores only exceptions that are `ExBase` or publicly derived from it
template<typename ExBase>
class [[nodiscard]] polymorphic_exception_storage // TODO: come up with a better name?
                                                  // because `int` is suitable too
{
    static_assert(
        std::is_same_v<ExBase, std::remove_cvref_t<ExBase>>,
        "`ExBase` must be cvref-unqualified"
    );

public:
    template<typename FwdEx = ExBase, typename Ex = std::remove_cvref_t<FwdEx>>
    requires std::is_convertible_v<Ex *, ExBase *> &&
             (!std::is_same_v<Ex, polymorphic_exception_storage>)
    explicit polymorphic_exception_storage(FwdEx &&fwd_ex)
        : storage_{std::forward<FwdEx>(fwd_ex)}
    {
    }

public:
    [[noreturn]] void throw_exception() const { storage_.throw_exception(); }

private:
    // cannot use inheritance because the invariant may be violated:
    //
    // ```
    // void violate_invariants(polymorhpic_exception_storage<std::exception> &f)
    // {
    //     polymorhpic_exception_storage<std::exception> tmp{std::exception{}};
    //     exception_storage &evil = tmp;
    //     evil = exception_storage{0};
    //     f = tmp; // boom!
    // }
    // ```
    exception_storage storage_;
};

template<typename Ex>
polymorphic_exception_storage(Ex) -> polymorphic_exception_storage<Ex>;

} // namespace siga::util
