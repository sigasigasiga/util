#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

#include <siga/util/meta.hpp>
#include <siga/util/utility.hpp>

namespace siga::util {

template<typename T>
class [[nodiscard]] storage_base : private no_unique_address_if_empty<T>
{
private:
    static_assert(!std::same_as<T, std::in_place_t>);
    using storage = no_unique_address_if_empty<T>;

public:
    // NOTE: all these constructors are heavily inspired by the constructors of `std::optional`.
    // Each constructor is annotated according to the `optional`'s constructor on the cppreference:
    // https://en.cppreference.com/w/cpp/utility/optional/optional

    // (1)
    constexpr storage_base() = default;

    // (2)
    constexpr storage_base(const storage_base &) = default;

    // (3)
    constexpr storage_base(storage_base &&) = default;

    // (4)
    // clang-format off
    template<typename U>
    requires
        std::is_constructible_v<T, const U &> &&
        (!std::is_constructible_v<T, storage_base<U>&>) &&
        (!std::is_constructible_v<T, const storage_base<U>&>) &&
        (!std::is_constructible_v<T, storage_base<U>&&>) &&
        (!std::is_constructible_v<T, const storage_base<U>&&>) &&
        (!std::is_convertible_v<storage_base<U>&, T>) &&
        (!std::is_convertible_v<const storage_base<U>&, T>) &&
        (!std::is_convertible_v<storage_base<U>&&, T>) &&
        (!std::is_convertible_v<const storage_base<U>&&, T>)
    explicit(!std::is_convertible_v<T, const U &>)
    constexpr
    storage_base(const storage_base<U> &value)
        noexcept(std::is_nothrow_copy_constructible_v<T>)
        : storage{ .value_ = value.get() }
    {
    }
    // clang-format on

    // (5)
    // clang-format off
    template<typename U>
    requires
        std::is_constructible_v<T, const U &> &&
        (!std::is_constructible_v<T, storage_base<U>&>) &&
        (!std::is_constructible_v<T, const storage_base<U>&>) &&
        (!std::is_constructible_v<T, storage_base<U>&&>) &&
        (!std::is_constructible_v<T, const storage_base<U>&&>) &&
        (!std::is_convertible_v<storage_base<U>&, T>) &&
        (!std::is_convertible_v<const storage_base<U>&, T>) &&
        (!std::is_convertible_v<storage_base<U>&&, T>) &&
        (!std::is_convertible_v<const storage_base<U>&&, T>)
    explicit(!std::is_convertible_v<T, U &&>)
    constexpr
    storage_base(storage_base<U> &&value)
        noexcept(std::is_nothrow_move_constructible_v<T>)
        : storage{ .value_ = std::move(value).get() }
    {
    }
    // clang-format on

    // (6)
    template<typename... Args>
    requires std::is_constructible_v<T, Args...>
    constexpr explicit storage_base(std::in_place_t, Args &&...args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>) // strengthened
        : storage{.value_ = T(std::forward<Args>(args)...)}
    {
    }

    // (7)
    template<typename U, typename... Args>
    requires std::is_constructible_v<T, std::initializer_list<U> &, Args...>
    constexpr explicit storage_base(std::in_place_t, std::initializer_list<U> ilist, Args &&...args)
        noexcept(
            std::is_nothrow_constructible_v<T, std::initializer_list<U> &, Args...> // strengthened
        )
        : storage{.value_ = T(ilist, std::forward<Args>(args)...)}
    {
    }

    // (8)
    // clang-format off
    template<typename U = T>
    requires
        std::is_constructible_v<T, U &&> &&
        (!std::same_as<std::remove_cvref_t<U>, std::in_place_t>) &&
        (!std::same_as<std::remove_cvref_t<U>, storage_base>)
    explicit(!std::is_convertible_v<U &&, T>)
    constexpr
    storage_base(U &&value)
        noexcept(std::is_nothrow_constructible_v<T, U>) // strengthened
        : storage{ .value_ = T(std::forward<U>(value)) }
    {
    }
    // clang-format on

protected:
    template<typename Self>
    constexpr auto &&value(this Self &&self) noexcept
    {
        return forward_self<Self, storage_base>(self).value_;
    }
};

// As in `optional`, we have a single deducing guide here
// https://en.cppreference.com/w/cpp/utility/optional/deduction_guides
template<typename T>
storage_base(T) -> storage_base<T>;

} // namespace siga::util
