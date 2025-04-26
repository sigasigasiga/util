#pragma once

#include <algorithm>
#include <format>
#include <functional>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>

namespace grace::util {

// https://stackoverflow.com/q/68798717/10961484

// Notes:
// 1. It works on each major implementation (gcc, clang, msvc),
//    though I'm not sure if this is even conforming.
// 2. `template<class = decltype(make_unique_type())> void f()` will be instantiated only once
//    on every compiler except MSVC https://godbolt.org/z/zrhP1oqK9
template<typename Unique = decltype([] {})>
[[nodiscard]] constexpr Unique make_unique_type() noexcept;

static_assert(not std::same_as<decltype(make_unique_type()), decltype(make_unique_type())>);

// Notes:
// 1. It works on each major implementation
// 2. `template<auto = make_unique_value()> void f()` will be instantiated only ONCE
//    on every compiler https://godbolt.org/z/Y9E385bnr
template<auto V = ([] {})>
[[nodiscard]] constexpr auto make_unique_value() noexcept
{
    return V;
}

static_assert(make_unique_value() != make_unique_value());

// Notes:
// 1. It works on each major implementation (gcc, clang, msvc)
//    and I'm pretty much assured that this is conforming
// 2. Because it requires to write empty angle brackets (`unique_type_t<>`)
//    it is easier to fuck it up by writing something inside them
// 3. `template<class = unique_type_t<>> void f()` will be instantiated again
//     on every new call on gcc and msvc but not on clang https://godbolt.org/z/15jr5hM8W
template<typename T = decltype([] {})>
using unique_type_t = T;

static_assert(not std::same_as<unique_type_t<>, unique_type_t<>>);

// Notes:
// 1. It works on each major implementation (gcc, clang, msvc)
//    and I'm pretty much assured that this is conforming
// 2. Because it requires to write empty angle brackets (`unique_value<>`)
//    it is easier to fuck it up by writing something inside them
// 3. `template<auto = unique_value<>> void f()` will be instantiated again
//     on every new use only on gcc https://godbolt.org/z/rxYPWcdsK
template<auto V = ([] {})>
constexpr auto unique_value = V;

static_assert(unique_value<> != unique_value<>);

// -------------------------------------------------------------------------------------------------

// Each unique call to this function produces a unique static object,
// thanks to the lambda-parametrized template parameter.
//
// This function probably doesn't have much use in practice
template<typename T, auto = [] {}, typename... Args>
requires std::constructible_from<T, Args &&...>
[[nodiscard]] T &make_static_object(Args &&...args)
    noexcept(std::is_nothrow_constructible_v<T, Args &&...>)
{
    static T ret(std::forward<Args>(args)...);
    return ret;
}

// -------------------------------------------------------------------------------------------------

// Each unique call to this function produces a shared pointer to a static object.
// The way it works is similar to `make_static_object` function
template<typename T, auto = [] {}, typename... Args>
requires std::constructible_from<T, Args &&...>
[[nodiscard]] std::shared_ptr<T> make_static_shared_ptr(Args &&...args)
    noexcept(std::is_nothrow_constructible_v<T, Args &&...>)
{
    static T ret(std::forward<Args>(args)...);

    // Aliasing constructor takes a control block pointer from the first arg
    // and a data pointer from the second.
    //
    // By passing a default-constructed shared pointer to it,
    // we can get a shared pointer that'd behave as a regular pointer:
    // 1. It wouldn't own the underlying object
    // 2. It wouldn't increment atomic counter on copy
    //
    // Don't try to construct `weak_ptr` from this pointer though
    return std::shared_ptr<T>(std::shared_ptr<void>(), &ret);
}

// -------------------------------------------------------------------------------------------------

// Behaves like `std::invoke`, except that it expects `std::weak_ptr` as the second argument
// and either invokes the function with underlying value or returns `Ret()`.
//
// It may be especially useful in asynchronous programming (e.g. with boost.asio)
class weak_invoke_t
{
public:
    // TODO: maybe we should prohibit non-`void` return types?
    template<
        typename Fn,
        typename T,
        typename... Args,
        typename Ret = std::invoke_result_t<Fn, T &, Args...>>
    requires std::invocable<Fn, T &, Args...> && requires { Ret(); }
    static Ret operator()(Fn &&fn, const std::weak_ptr<T> &weak, Args &&...args)
        noexcept(std::is_nothrow_invocable_v<Fn, T &, Args...> && noexcept(Ret()))
    {
        if(auto p = weak.lock()) {
            return std::invoke(std::forward<Fn>(fn), *p, std::forward<Args>(args)...);
        } else {
            return Ret();
        }
    }
};

inline constexpr weak_invoke_t weak_invoke;

// -------------------------------------------------------------------------------------------------

// I thought that this struct would be useful for producing unique type,
// but for some reason it didn't work (see `print_nttp_source_location`)
template<std::size_t MaxStringSize = 256>
struct nttp_source_location
{
public:
    constexpr nttp_source_location(std::source_location loc)
        : raw_file_name{0}
        , raw_function_name{0}
    {
        std::string_view file_name(loc.file_name()), function_name(loc.function_name());
        if(file_name.size() >= MaxStringSize || function_name.size() >= MaxStringSize) {
            throw std::length_error{"The strings are too big"};
        }

        std::ranges::copy(file_name, raw_file_name.begin());
        std::ranges::copy(function_name, raw_function_name.begin());
        raw_line = loc.line();
        raw_column = loc.column();
    }

public:
    constexpr std::string_view file_name() const noexcept { return raw_file_name.begin(); }
    constexpr std::string_view function_name() const noexcept { return raw_function_name.begin(); }
    constexpr std::uint_least32_t line() const noexcept { return raw_line; }
    constexpr std::uint_least32_t column() const noexcept { return raw_column; }

public:
    std::array<char, MaxStringSize> raw_file_name;
    std::array<char, MaxStringSize> raw_function_name;
    std::uint_least32_t raw_line;
    std::uint_least32_t raw_column;
};

std::string format_source_location(const auto &loc)
{
    return std::format(
        "{}({}:{}) `{}`",
        loc.file_name(),
        loc.line(),
        loc.column(),
        loc.function_name()
    );
}

#if 0
// doesn't work, prints out the line number of the template parameter,
// not the line number of the function caller
template<nttp_source_location Loc = std::source_location::current()>
void print_nttp_source_location()
{
    std::println("{}", format_source_location(Loc));
}
#endif

} // namespace grace::util
