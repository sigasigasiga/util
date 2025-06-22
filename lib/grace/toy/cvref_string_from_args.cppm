module;

#include <string_view>

export module grace.toy:cvref_string_from_args;

export namespace grace::toy {

constexpr std::string_view cvref_string_from_args(auto &) noexcept { return "auto &"; }
constexpr std::string_view cvref_string_from_args(const auto &) noexcept { return "const auto &"; }
constexpr std::string_view cvref_string_from_args(volatile auto &) noexcept { return "volatile auto &"; }
constexpr std::string_view cvref_string_from_args(const volatile auto &) noexcept { return "const volatile auto &"; }
constexpr std::string_view cvref_string_from_args(auto &&) noexcept { return "auto &&"; }
constexpr std::string_view cvref_string_from_args(const auto &&) noexcept { return "const auto &&"; }
constexpr std::string_view cvref_string_from_args(volatile auto &&) noexcept { return "volatile auto &&"; }
constexpr std::string_view cvref_string_from_args(const volatile auto &&) noexcept { return "const volatile auto &&"; }

} // namespace grace::toy
