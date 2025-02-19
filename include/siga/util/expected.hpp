#pragma once

#include <expected>
#include <functional>
#include <optional>

namespace siga::util {

// This is customizable in order to use `beman::optional26`
template<template<typename...> typename Optional = std::optional>
class [[nodiscard]] ok_or_t
{
public:
    template<typename T, typename E>
    [[nodiscard]] static constexpr std::expected<T, E> operator()(Optional<T> &value, E &&err)
    {
        if(value) {
            return *value;
        } else {
            return std::unexpected{std::forward<E>(err)};
        }
    }

    template<typename T, typename E>
    [[nodiscard]] static constexpr std::expected<T, E> operator()(const Optional<T> &value, E &&err)
    {
        if(value) {
            return *value;
        } else {
            return std::unexpected{std::forward<E>(err)};
        }
    }

    template<typename T, typename E>
    [[nodiscard]] static constexpr std::expected<T, E> operator()(Optional<T> &&value, E &&err)
    {
        if(value) {
            return *std::move(value);
        } else {
            return std::unexpected{std::forward<E>(err)};
        }
    }

    template<typename T, typename E>
    [[nodiscard]] static constexpr auto operator()(const Optional<T> &&value, E &&err) //
        -> std::expected<T, E>
    {
        if(value) {
            return *std::move(value);
        } else {
            return std::unexpected{std::forward<E>(err)};
        }
    }
};

inline constexpr ok_or_t ok_or;

// This is customizable in order to use `beman::optional26`
template<template<typename...> typename Optional = std::optional>
class [[nodiscard]] ok_or_else_t
{
public:
    template<typename T, typename ErrF, typename ErrT = std::invoke_result_t<ErrF>>
    [[nodiscard]] static constexpr std::expected<T, ErrT> operator()(Optional<T> &value, ErrF &&fn)
    {
        if(value) {
            return *value;
        } else {
            return std::unexpected{std::invoke(std::forward<ErrF>(fn))};
        }
    }

    template<typename T, typename ErrF, typename ErrT = std::invoke_result_t<ErrF>>
    [[nodiscard]] static constexpr auto operator()(const Optional<T> &value, ErrF &&fn) //
        -> std::expected<T, ErrT>
    {
        if(value) {
            return *value;
        } else {
            return std::unexpected{std::invoke(std::forward<ErrF>(fn))};
        }
    }

    template<typename T, typename ErrF, typename ErrT = std::invoke_result_t<ErrF>>
    [[nodiscard]] static constexpr std::expected<T, ErrT> operator()(Optional<T> &&value, ErrF &&fn)
    {
        if(value) {
            return *std::move(value);
        } else {
            return std::unexpected{std::invoke(std::forward<ErrF>(fn))};
        }
    }

    template<typename T, typename ErrF, typename ErrT = std::invoke_result_t<ErrF>>
    [[nodiscard]] static constexpr auto operator()(const Optional<T> &&value, ErrF &&fn) //
        -> std::expected<T, ErrT>
    {
        if(value) {
            return *std::move(value);
        } else {
            return std::unexpected{std::invoke(std::forward<ErrF>(fn))};
        }
    }
};

inline constexpr ok_or_else_t ok_or_else;

} // namespace siga::util
