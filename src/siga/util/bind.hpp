#pragma once

#include <functional>
#include <type_traits>
#include <tuple>

#include <siga/util/functional.hpp>
#include <siga/util/utility.hpp>

namespace siga::util {

namespace detail {

#ifdef __cpp_lib_bind_back

template<typename... Args>
constexpr auto bind_back(Args &&...args)
    noexcept((... && is_nothrow_decay_copy_constructible_v<Args>))
{
    return std::bind_back(std::forward<Args>(args)...);
}

#else // __cpp_lib_bind_back

// TODO: This should be `noexcept`, however this is a temporary fix until LLVM 19
// so it probably doesn't worth the effort
template<typename Fn, typename... Bound>
requires std::is_object_v<Fn> && (... && std::is_object_v<Bound>)
class [[nodiscard]] bind_back_impl
{
public:
    template<typename UFn, typename... UBound>
    constexpr bind_back_impl(UFn &&fn, UBound &&...bound)
        : fn_{std::forward<UFn>(fn)}
        , storage_{std::forward<UBound>(bound)...}
    {
    }

public:
    template<typename Self, typename... Args>
    constexpr decltype(auto) operator()(this Self &&self, Args &&...args)
    {
        return std::apply(
            std::forward<Self>(self).fn_,
            std::tuple_cat(
                std::forward_as_tuple(std::forward<Args>(args)...),
                std::forward<Self>(self).storage_
            )
        );
    }

private:
    Fn fn_;
    std::tuple<Bound...> storage_;
};

template<typename... Args>
constexpr auto bind_back(Args &&...args)
    noexcept((... && is_nothrow_decay_copy_constructible_v<Args>))
{
    return bind_back_impl<std::decay_t<Args>...>(std::forward<Args>(args)...);
}

#endif // __cpp_lib_bind_back

} // namespace detail

// -------------------------------------------------------------------------------------------------

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_front_unwrap(F &&f, Args &&...args) //
    noexcept(
        is_nothrow_decay_copy_constructible_v<F> &&
        (... && is_nothrow_decay_copy_constructible_v<Args>)
    )
{
    return std::bind_front(get_reference_wrap(std::forward<F>(f)), std::forward<Args>(args)...);
}

template<typename F, typename... Args>
[[nodiscard]] constexpr auto bind_back_unwrap(F &&f, Args &&...args) //
    noexcept(
        is_nothrow_decay_copy_constructible_v<F> &&
        (... && is_nothrow_decay_copy_constructible_v<Args>)
    )
{
    // TODO: use `std::bind_back` when llvm 19
    return detail::bind_back(get_reference_wrap(std::forward<F>(f)), std::forward<Args>(args)...);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto equal_to(T &&value)
    noexcept(noexcept(bind_front_unwrap(std::equal_to(), std::forward<T>(value))))
{
    return bind_front_unwrap(std::equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename T>
[[nodiscard]] constexpr auto not_equal_to(T &&value)
    noexcept(noexcept(bind_front_unwrap(std::not_equal_to(), std::forward<T>(value))))
{
    return bind_front_unwrap(std::not_equal_to(), std::forward<T>(value));
}

// -------------------------------------------------------------------------------------------------

template<typename Container>
[[nodiscard]] constexpr auto index_in(Container &&container)
    noexcept(noexcept(bind_front_unwrap(subscript(), std::forward<Container>(container))))
{
    return bind_front_unwrap(subscript(), std::forward<Container>(container));
}

// -------------------------------------------------------------------------------------------------

template<typename Idx>
[[nodiscard]] constexpr auto index_with(Idx &&idx)
    noexcept(noexcept(bind_back_unwrap(subscript(), std::forward<Idx>(idx))))
{
    return bind_back_unwrap(subscript(), std::forward<Idx>(idx));
}

} // namespace siga::util
