#pragma once

#include <functional>
#include <tuple>

#ifdef _MSC_VER
#    define SIGA_UTIL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else // _MSC_VER
#    define SIGA_UTIL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif // _MSC_VER

// -------------------------------------------------------------------------------------------------

namespace siga::util {

namespace detail {

#ifdef __cpp_lib_bind_back

template<typename... Args>
constexpr auto my_bind_back(Args &&...args)
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
constexpr auto my_bind_back(Args &&...args)
{
    return bind_back_impl<std::decay_t<Args>...>(std::forward<Args>(args)...);
}

#endif // __cpp_lib_bind_back

} // namespace detail

template<typename... Args>
constexpr auto bind_back(Args &&...args)
{
    return detail::my_bind_back(std::forward<Args>(args)...);
}

} // namespace siga::util
