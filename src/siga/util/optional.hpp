#pragma once

#include <optional>
#include <utility>

#include <siga/util/utility.hpp>

namespace siga::util {

class [[nodiscard]] as_optional_t
{
public:
    template<std::copy_constructible T>
    [[nodiscard]] static constexpr std::optional<std::decay_t<T>> operator()(T *ptr)
        noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        if(ptr) {
            return std::make_optional(*ptr);
        } else {
            return std::nullopt;
        }
    }
};

inline constexpr as_optional_t as_optional;

// -------------------------------------------------------------------------------------------------

class [[nodiscard]] as_optional_move_t
{
public:
    template<std::move_constructible T>
    [[nodiscard]] static constexpr std::optional<std::decay_t<T>> operator()(T *ptr)
        noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        if(ptr) {
            return std::make_optional(std::move(*ptr));
        } else {
            return std::nullopt;
        }
    }
};

inline constexpr as_optional_move_t as_optional_move;

// -------------------------------------------------------------------------------------------------

template<typename Like>
class [[nodiscard]] as_optional_like_t
{
public:
    template<decay_copy_constructible T>
    [[nodiscard]] static constexpr std::optional<std::decay_t<T>> operator()(T *ptr)
        noexcept(is_nothrow_decay_copy_constructible_v<T>)
    {
        if(ptr) {
            return std::make_optional(std::forward_like<Like>(*ptr));
        } else {
            return std::nullopt;
        }
    }
};

template<typename Like>
constexpr as_optional_like_t<Like> as_optional_like;

} // namespace siga::util
