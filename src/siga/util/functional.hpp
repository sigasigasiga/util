#pragma once

#include <concepts>
#include <functional>
#include <utility>

#include <siga/util/meta.hpp>
#include <siga/util/storage_base.hpp>
#include <siga/util/tuple.hpp>
#include <siga/util/utility.hpp>

namespace siga::util {

template<typename T, bool UseRoundBrackets = true>
class [[nodiscard]] construct_t
{
public:
    template<typename... Args>
    requires std::constructible_from<T, Args &&...>
    [[nodiscard]] static constexpr T operator()(Args &&...args)
        noexcept(std::is_nothrow_constructible_v<T, Args &&...>)
    {
        return T(std::forward<Args>(args)...);
    }
};

template<typename T>
class [[nodiscard]] construct_t<T, false>
{
public:
    template<typename... Args>
    requires requires(Args &&...args) { T{std::forward<Args>(args)...}; }
    [[nodiscard]] static constexpr T operator()(Args &&...args)
        noexcept(noexcept(T{std::forward<Args>(args)...}))
    {
        return T{std::forward<Args>(args)...};
    }
};

template<typename T, bool UseRoundBrackets = true>
inline constexpr construct_t<T, UseRoundBrackets> construct;

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] static_value_cast_t
{
public:
    template<typename From>
    requires requires(From &&from) { static_cast<To>(std::forward<From>(from)); }
    [[nodiscard]] static constexpr To operator()(From &&from)
        noexcept(noexcept(static_cast<To>(std::forward<From>(from))))
    {
        return static_cast<To>(std::forward<From>(from));
    }
};

template<typename To>
inline constexpr static_value_cast_t<To> static_value_cast;

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] dynamic_value_cast_t
{
public:
    template<typename From>
    requires requires(From &&from) { dynamic_cast<To>(std::forward<From>(from)); }
    [[nodiscard]] static constexpr To operator()(From &&from)
        noexcept(noexcept(dynamic_cast<To>(std::forward<From>(from))))
    {
        return dynamic_cast<To>(std::forward<From>(from));
    }
};

template<typename To>
inline constexpr dynamic_value_cast_t<To> dynamic_value_cast;

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] const_value_cast_t
{
public:
    template<typename From>
    requires requires(From &&from) { const_cast<To>(std::forward<From>(from)); }
    [[nodiscard]] static constexpr To operator()(From &&from) noexcept
    {
        return const_cast<To>(std::forward<From>(from));
    }
};

template<typename To>
inline constexpr const_value_cast_t<To> const_value_cast;

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] reinterpret_value_cast_t
{
public:
    template<typename From>
    requires requires(From &&from) { reinterpret_cast<To>(std::forward<From>(from)); }
    [[nodiscard]] static constexpr To operator()(From &&from) noexcept
    {
        return reinterpret_cast<To>(std::forward<From>(from));
    }
};

template<typename To>
inline constexpr reinterpret_value_cast_t<To> reinterpret_value_cast;

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] c_style_cast_t
{
public:
    template<typename From>
    requires requires(From &&from) { (To) std::forward<From>(from); }
    [[nodiscard]] static constexpr To operator()(From &&from)
        noexcept(noexcept((To)std::forward<From>(from)))
    {
        return (To)std::forward<From>(from);
    }
};

template<typename To>
inline constexpr c_style_cast_t<To> c_style_cast;

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] indirect_t
{
public:
    template<typename T>
    requires requires(T &&object) { *std::forward<T>(object); }
    [[nodiscard]] static constexpr decltype(auto) operator()(T &&object)
        noexcept(noexcept(*std::forward<T>(object)))
    {
        return *std::forward<T>(object);
    }
};

inline constexpr indirect_t indirect;

// -------------------------------------------------------------------------------------------------

template<typename F>
class [[nodiscard]] lazy_eval : private storage_base<F>
{
public:
    using storage_base<F>::storage_base;

public:
    template<typename Self>
    [[nodiscard]] constexpr operator std::invoke_result_t<copy_cvref_t<Self, F>>(this Self &&self)
        noexcept(std::is_nothrow_invocable_v<copy_cvref_t<Self, F>>)
    {
        return std::invoke(forward_self<Self, lazy_eval>(self).value());
    }
};

template<typename F>
lazy_eval(F) -> lazy_eval<F>;

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] get_reference_t
{
public:
    template<typename T>
    [[nodiscard]] static constexpr decltype(auto) operator()(T &&value) noexcept
    {
        return std::forward<T>(value);
    }

    template<typename T>
    [[nodiscard]] static constexpr decltype(auto) operator()(std::reference_wrapper<T> ref) noexcept
    {
        return ref.get();
    }
};

inline constexpr get_reference_t get_reference;

// -------------------------------------------------------------------------------------------------

template<typename F>
class [[nodiscard]] get_reference_wrap : private storage_base<F>
{
public:
    using storage_base<F>::storage_base;

public:
    // clang-format off
    template<typename Self, typename... Args>
    requires requires(Self &&self, Args &&...args) {
        std::invoke(
            forward_self<Self, get_reference_wrap>(self).value(),
            get_reference(std::forward<Args>(args))...
        );
    }
    constexpr decltype(auto) operator()(this Self &&self, Args &&...args)
        noexcept(noexcept(std::invoke(
            forward_self<Self, get_reference_wrap>(self).value(),
            get_reference(std::forward<Args>(args))...
        )))
    {
        return std::invoke(
            forward_self<Self, get_reference_wrap>(self).value(),
            get_reference(std::forward<Args>(args))...
        );
    }
    // clang-format on
};

template<typename F>
get_reference_wrap(F) -> get_reference_wrap<F>;

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] subscript
{
public:
    template<typename L, typename R>
    requires requires(L &&lhs, R &&rhs) { std::forward<L>(lhs)[std::forward<R>(rhs)]; }
    [[nodiscard]] static constexpr decltype(auto) operator()(L &&lhs, R &&rhs)
        noexcept(noexcept(std::forward<L>(lhs)[std::forward<R>(rhs)]))
    {
        return std::forward<L>(lhs)[std::forward<R>(rhs)];
    }
};

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] throw_exception
{
public:
    template<typename T>
    [[noreturn]] void operator()(T &&value)
    {
        throw std::forward<T>(value);
    }
};

// -------------------------------------------------------------------------------------------------

template<typename T, typename R = T>
class [[nodiscard]] return_value : private storage_base<T>
{
public:
    using value_type = T; // TODO: is it needed? the user can't access the value directly anyway
    using return_type = R;

public:
    using storage_base<T>::storage_base;

public:
    template<typename Self>
    [[nodiscard]] constexpr return_type operator()(this Self &&self)
        noexcept(std::is_nothrow_constructible_v<return_type, copy_cvref_t<Self, T>>)
    {
        return forward_self<Self, return_value>(self).value();
    }
};

template<>
class [[nodiscard]] return_value<void>
{
public:
    using value_type = void; // TODO: is it needed? the user can't access the value directly anyway
    using return_type = void;

public:
    constexpr return_type operator()() noexcept {}
};

template<typename T>
return_value(T) -> return_value<T>;

/// Deduce `T` and remove its cvref.
/// If `R` is not present, it's the same as `T`
template<
    typename R = struct secret_impl_tag,
    typename T,
    typename ValueType = std::remove_cvref_t<T>,
    typename ReturnType = std::conditional_t<std::is_same_v<R, secret_impl_tag>, ValueType, R>,
    typename ResultType = return_value<ValueType, ReturnType>>
[[nodiscard]] constexpr auto make_return_value(T &&value)
    noexcept(std::is_nothrow_constructible_v<ResultType, T &&>) //
    -> ResultType
{
    return ResultType(std::forward<T>(value));
}

/// Deduce `T` and remove its cvref, and set `R` to `TypeToRetTrait<T>::type`
template<
    template<typename...> typename TypeToRetTrait,
    typename T,
    typename ValueType = std::remove_cvref_t<T>,
    typename ReturnType = typename TypeToRetTrait<ValueType>::type,
    typename ResultType = return_value<ValueType, ReturnType>>
[[nodiscard]] constexpr auto make_return_value(T &&value)
    noexcept(std::is_nothrow_constructible_v<ResultType, T &&>) //
    -> ResultType
{
    return ResultType(std::forward<T>(value));
}

/// Same as `return_value<void>()`, added for consistency
template<conceptify<std::is_void> T>
[[nodiscard]] constexpr auto make_return_value() noexcept
{
    return return_value<void>();
}

// -------------------------------------------------------------------------------------------------

template<std::invocable F>
class [[nodiscard]] ignore_args_wrap : private storage_base<F>
{
public:
    using storage_base<F>::storage_base;

public:
    template<typename Self>
    requires std::invocable<copy_cvref_t<Self, F>>
    constexpr decltype(auto) operator()(this Self &&self, auto &&...)
        noexcept(std::is_nothrow_invocable_v<copy_cvref_t<Self, F>>)
    {
        return std::invoke(forward_self<Self, ignore_args_wrap>(self).value());
    }
};

template<typename F>
ignore_args_wrap(F) -> ignore_args_wrap<F>;

// -------------------------------------------------------------------------------------------------

template<typename F>
class [[nodiscard]] stored_func_invoker : private storage_base<F>
{
public:
    using storage_base<F>::storage_base;

public:
    template<typename Self, typename... Args>
    requires std::invocable<copy_cvref_t<Self, F>, Args...>
    constexpr decltype(auto) operator()(this Self &&self, Args &&...args)
        noexcept(std::is_nothrow_invocable_v<copy_cvref_t<Self, F>, Args...>)
    {
        return std::invoke(
            forward_self<Self, stored_func_invoker>(self).value(),
            std::forward<Args>(args)...
        );
    }
};

template<typename F>
stored_func_invoker(F) -> stored_func_invoker<F>;

// -------------------------------------------------------------------------------------------------

template<typename FsTuple, typename... InnerArgs>
class is_nothrow_fold_invocable
{
private:
    template<typename IndexSequence, typename... Args>
    class impl;

    template<std::size_t I, typename... Args>
    class impl<std::index_sequence<I>, Args...>
    {
    private:
        using current_fn_t = tuple_get_type_t<FsTuple, I>;

    public:
        static constexpr bool value = std::is_nothrow_invocable_v<current_fn_t, Args...>;
    };

    template<std::size_t I, std::size_t... Is, typename... Args>
    class impl<std::index_sequence<I, Is...>, Args...>
    {
    private:
        using current_fn_t = tuple_get_type_t<FsTuple, I>;
        using current_ret_t = std::invoke_result_t<current_fn_t, Args...>;

        static constexpr bool current = std::is_nothrow_invocable_v<current_fn_t, Args...>;

    public:
        static constexpr bool value =
            current && impl<std::index_sequence<Is...>, current_ret_t>::value;
    };

private:
    static constexpr std::size_t tuple_size = std::tuple_size_v<std::decay_t<FsTuple>>;

public:
    static constexpr bool value = impl<std::make_index_sequence<tuple_size>, InnerArgs...>::value;
};

template<typename FsTuple, typename... Args>
inline constexpr bool is_nothrow_fold_invocable_v =
    is_nothrow_fold_invocable<FsTuple, Args...>::value;

class [[nodiscard]] fold_invoke_t
{
public:
    template<typename FuncTuple, typename... Args>
    static constexpr decltype(auto) operator()(FuncTuple &&funcs, Args &&...args)
        noexcept(is_nothrow_fold_invocable_v<FuncTuple &&, Args &&...>)
    {
        return impl(
            std::make_index_sequence<std::tuple_size_v<std::decay_t<FuncTuple>>>{},
            std::forward<FuncTuple>(funcs),
            std::forward<Args>(args)...
        );
    }

private:
    // clang-format off
    template<typename FsTuple, std::size_t I, std::size_t... Is, typename... Args>
    requires std::invocable<tuple_get_type_t<FsTuple &&, I>, Args &&...>
    static constexpr decltype(auto) impl(
        std::index_sequence<I, Is...>,
        FsTuple &&fs,
        Args &&...args
    ) {
        if constexpr(sizeof...(Is) == 0) {
            return std::invoke(get<I>(std::forward<FsTuple>(fs)), std::forward<Args>(args)...);
        } else {
            return impl(
                std::index_sequence<Is...>{},
                std::forward<FsTuple>(fs),
                std::invoke(get<I>(std::forward<FsTuple>(fs)), std::forward<Args>(args)...)
            );
        }
    }
    // clang-format on
};

inline constexpr fold_invoke_t fold_invoke;

template<typename... Fs>
[[nodiscard]] constexpr auto compose(Fs &&...fs)
    noexcept((... && is_nothrow_decay_copy_constructible_v<Fs>))
{
    return std::bind_front(fold_invoke, std::tuple{std::forward<Fs>(fs)...});
}

// -------------------------------------------------------------------------------------------------

template<typename T>
class [[nodiscard]] typed_get
{
public:
    template<typename Gettable>
    requires requires(Gettable &&gettable) { get<T>(std::forward<Gettable>(gettable)); }
    [[nodiscard]] static constexpr decltype(auto) operator()(Gettable &&gettable)
        noexcept(noexcept(get<T>(std::declval<Gettable>())))
    {
        return get<T>(std::forward<Gettable>(gettable));
    }
};

template<auto V>
class [[nodiscard]] valued_get
{
public:
    template<typename Gettable>
    requires requires(Gettable &&gettable) { get<V>(std::forward<Gettable>(gettable)); }
    [[nodiscard]] static constexpr decltype(auto) operator()(Gettable &&gettable)
        noexcept(noexcept(get<V>(std::declval<Gettable>())))
    {
        return get<V>(std::forward<Gettable>(gettable));
    }
};

template<typename T>
constexpr typed_get<T> make_get() noexcept
{
    return {};
}

template<auto V>
constexpr valued_get<V> make_get() noexcept
{
    return {};
}

inline constexpr auto get_key = make_get<0>();
inline constexpr auto get_value = make_get<1>();

// -------------------------------------------------------------------------------------------------

template<typename... Ts>
class [[nodiscard]] overload : public Ts...
{
public:
    using Ts::operator()...;
};

// -------------------------------------------------------------------------------------------------

template<typename F>
class [[nodiscard]] make_bind_expression : public stored_func_invoker<F>
{};

template<typename F>
make_bind_expression(F) -> make_bind_expression<F>;

// -------------------------------------------------------------------------------------------------

} // namespace siga::util

template<typename F>
class std::is_bind_expression<siga::util::make_bind_expression<F>> : public std::true_type
{};

// -------------------------------------------------------------------------------------------------

// clang-format off
#define SIGA_UTIL_LIFT_FUNCTION_SINGLE(X)                                                          \
    []<typename... Args>(Args &&...args)                                                           \
        constexpr                                                                                  \
        static                                                                                     \
        noexcept(noexcept(X(::std::forward<Args>(args)...)))                                       \
        -> decltype(auto)                                                                          \
        requires requires { X(::std::forward<Args>(args)...); }                                    \
    {                                                                                              \
        return X(::std::forward<Args>(args)...);                                                   \
    }
// clang-format on

#define SIGA_UTIL_AS_SINGLE(...) __VA_ARGS__

#define SIGA_UTIL_LIFT_FUNCTION(...)                                                               \
    SIGA_UTIL_LIFT_FUNCTION_SINGLE(SIGA_UTIL_AS_SINGLE(__VA_ARGS__))

// -------------------------------------------------------------------------------------------------

// clang-format off

// Known limitations:
// 1. Resulting lambda never produces an overload set.
//    That is, if passed to `util::overload`, the result may be surprising
// 2. If `MEMBER` is a niebloid, it'd be called, despite the fact
//    that `std::invoke` would return a reference to it.
//    While this is probably fixable, I'm not sure if it's worth the effort
// 
// TODO: should we also specify a class name? If so, don't forget that types must be _compatible_
#define SIGA_UTIL_LIFT_MEMBER_SINGLE(MEMBER) \
    []< \
        typename T, \
        typename... Args, \
        bool IsRefWrapper = ::std::same_as< \
            decltype(::std::ref(::std::declval<T &>())), \
            ::std::decay_t<T> \
        > \
    > \
        (T &&value, Args &&...args) \
        constexpr \
        static \
        noexcept( \
            [] { \
                if constexpr(requires { ::std::forward<T>(value).MEMBER(::std::forward<Args>(args)...); }) { \
                    return noexcept(::std::forward<T>(value).MEMBER(::std::forward<Args>(args)...)); \
                } else if constexpr(IsRefWrapper && requires { value.get().MEMBER(::std::forward<Args>(args)...); }) { \
                    return noexcept(value.get().MEMBER(::std::forward<Args>(args)...)); \
                } else if constexpr(requires { (*::std::forward<T>(value)).MEMBER(::std::forward<Args>(args)...); }) { \
                    return noexcept((*::std::forward<T>(value)).MEMBER(::std::forward<Args>(args)...)); \
                } else if constexpr(requires { (*::std::forward<T>(value)).MEMBER; }) { \
                    return noexcept(*::std::forward<T>(value)); \
                } else { \
                    return true; \
                } \
            }() \
        ) \
        -> decltype(auto) \
        requires \
            requires { ::std::forward<T>(value).MEMBER(::std::forward<Args>(args)...); } || \
            (IsRefWrapper && requires { value.get().MEMBER(::std::forward<Args>(args)...); }) || \
            requires { (*::std::forward<T>(value)).MEMBER(::std::forward<Args>(args)...); } || \
            requires { ::std::forward<T>(value).MEMBER; } || \
            (IsRefWrapper && requires { value.get().MEMBER; }) || \
            requires { (*::std::forward<T>(value)).MEMBER; } \
    { \
        if constexpr(requires { ::std::forward<T>(value).MEMBER(::std::forward<Args>(args)...); }) { \
            return ::std::forward<T>(value).MEMBER(::std::forward<Args>(args)...); \
        } else if constexpr(IsRefWrapper && requires { value.get().MEMBER(::std::forward<Args>(args)...); }) { \
            return value.get().MEMBER(::std::forward<Args>(args)...); \
        } else if constexpr(requires { (*::std::forward<T>(value)).MEMBER(::std::forward<Args>(args)...); }) { \
            return (*::std::forward<T>(value)).MEMBER(::std::forward<Args>(args)...); \
        } else if constexpr(requires { ::std::forward<T>(value).MEMBER; }) { \
            /* Parenthesis around needed for `decltype(auto)` */ \
            return (::std::forward<T>(value).MEMBER); \
        } else if constexpr(IsRefWrapper && requires { value.get().MEMBER; }) { \
            return (value.get().MEMBER); \
        } else if constexpr(requires { (*::std::forward<T>(value)).MEMBER; }) { \
            return ((*::std::forward<T>(value)).MEMBER); \
        } \
    }

// clang-format on

// https://stackoverflow.com/a/77860333/10961484
#define SIGA_UTIL_MAKE_MEMBER_DETAIL(MEMBER)   MEMBER
#define SIGA_UTIL_MAKE_MEMBER_DETAIL_MANY(...) template __VA_ARGS__

#define SIGA_UTIL_MAKE_MEMBER_NAME(FIRST, ...)                                                     \
    SIGA_UTIL_MAKE_MEMBER_DETAIL##__VA_OPT__(_MANY)(FIRST __VA_OPT__(, ) __VA_ARGS__)

#define SIGA_UTIL_LIFT_MEMBER(...)                                                                 \
    SIGA_UTIL_LIFT_MEMBER_SINGLE(SIGA_UTIL_MAKE_MEMBER_NAME(__VA_ARGS__))
