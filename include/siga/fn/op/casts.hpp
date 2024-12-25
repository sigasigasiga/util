#pragma once

#include <utility>

namespace siga::fn::op {

template<typename To>
class [[nodiscard]] static_value_cast
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

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] dynamic_value_cast
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

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] const_value_cast
{
public:
    template<typename From>
    requires requires(From &&from) { const_cast<To>(std::forward<From>(from)); }
    [[nodiscard]] static constexpr To operator()(From &&from) noexcept
    {
        return const_cast<To>(std::forward<From>(from));
    }
};

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] reinterpret_value_cast
{
public:
    template<typename From>
    requires requires(From &&from) { reinterpret_cast<To>(std::forward<From>(from)); }
    [[nodiscard]] static constexpr To operator()(From &&from) noexcept
    {
        return reinterpret_cast<To>(std::forward<From>(from));
    }
};

// -------------------------------------------------------------------------------------------------

template<typename To>
class [[nodiscard]] c_style_cast
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

} // namespace siga::fn::op
