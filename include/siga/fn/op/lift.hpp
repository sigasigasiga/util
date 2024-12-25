#pragma once

// clang-format off
#define SIGA_FN_OP_LIFT_FUNCTION_SINGLE(X)                                                          \
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

#define SIGA_FN_OP_AS_SINGLE(...) __VA_ARGS__

#define SIGA_FN_OP_LIFT_FUNCTION(...)                                                              \
    SIGA_FN_OP_LIFT_FUNCTION_SINGLE(SIGA_FN_OP_AS_SINGLE(__VA_ARGS__))

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
#define SIGA_FN_OP_LIFT_MEMBER_SINGLE(MEMBER) \
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
#define SIGA_FN_OP_MAKE_MEMBER_DETAIL(MEMBER)   MEMBER
#define SIGA_FN_OP_MAKE_MEMBER_DETAIL_MANY(...) template __VA_ARGS__

#define SIGA_FN_OP_MAKE_MEMBER_NAME(FIRST, ...)                                                    \
    SIGA_FN_OP_MAKE_MEMBER_DETAIL##__VA_OPT__(_MANY)(FIRST __VA_OPT__(, ) __VA_ARGS__)

#define SIGA_FN_OP_LIFT_MEMBER(...)                                                                \
    SIGA_FN_OP_LIFT_MEMBER_SINGLE(SIGA_FN_OP_MAKE_MEMBER_NAME(__VA_ARGS__))
