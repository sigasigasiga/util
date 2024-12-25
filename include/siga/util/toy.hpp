#pragma once

#include <iostream>

namespace siga::util {

template<typename...>
struct type_print;

template<auto...>
struct value_print;

template<typename... Ts>
type_print<Ts...> print();

template<typename... Ts>
type_print<Ts...> print(const Ts &...);

template<auto... Vs>
value_print<Vs...> print();

// -------------------------------------------------------------------------------------------------

class printing_constructor
{
public:
    printing_constructor() { std::cout << "printing_constructor(): " << this << std::endl; }

    printing_constructor(const printing_constructor &rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor(const printing_constructor &); "
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on
    }

    printing_constructor(printing_constructor &&rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor(printing_constructor &&); "
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on
    }

    printing_constructor &operator=(const printing_constructor &rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor &operator=(const printing_constructor &); " 
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on

        return *this;
    }

    printing_constructor &operator=(printing_constructor &&rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor &operator=(printing_constructor &&); " 
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on

        return *this;
    }

    ~printing_constructor() { std::cout << "~printing_constructor(): " << this << std::endl; }
};

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] cvref_type_string_t
{
public:
    // clang-format off
    constexpr static std::string_view operator()(auto &) noexcept { return "auto &"; }
    constexpr static std::string_view operator()(const auto &) noexcept { return "const auto &"; }
    constexpr static std::string_view operator()(volatile auto &) noexcept { return "volatile auto &"; }
    constexpr static std::string_view operator()(const volatile auto &) noexcept { return "const volatile auto &"; }
    constexpr static std::string_view operator()(auto &&) noexcept { return "auto &&"; }
    constexpr static std::string_view operator()(const auto &&) noexcept { return "const auto &&"; }
    constexpr static std::string_view operator()(volatile auto &&) noexcept { return "volatile auto &&"; }
    constexpr static std::string_view operator()(const volatile auto &&) noexcept { return "const volatile auto &&"; }
    // clang-format on
};

inline constexpr cvref_type_string_t cvref_type_string;

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] printing_invoker
{
public:
    template<typename Self>
    void operator()(this Self &&self)
    {
        std::cout << cvref_type_string(std::forward<Self>(self)) << std::endl;
    }
};

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] ignore_t
{
public:
    constexpr const ignore_t &operator=(auto &&) const noexcept { return *this; }
    constexpr void operator()(auto &&...) const noexcept {}
};

inline constexpr ignore_t ignore;

// -------------------------------------------------------------------------------------------------

template<typename T>
class [[nodiscard]] type_tag
{
public:
    using type = T;
};

template<auto V>
class [[nodiscard]] value_tag
{
public:
    constexpr static auto value = V;
};

template<typename T>
constexpr type_tag<T> make_tag() noexcept
{
    return {};
}

template<auto V>
constexpr value_tag<V> make_tag() noexcept
{
    return {};
}

// -------------------------------------------------------------------------------------------------

struct [[nodiscard]] empty
{};

} // namespace siga::util
