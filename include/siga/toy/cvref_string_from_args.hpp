#pragma once

#include <string_view>

namespace siga::toy {

class [[nodiscard]] cvref_string_from_args
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

} // namespace siga::toy
