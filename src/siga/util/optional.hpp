#pragma once

#include <optional>
#include <utility>

namespace siga::util {

class as_optional_t
{
public:
    template<typename T>
    static constexpr std::optional<T> operator()(T *ptr)
    {
        if(ptr) {
            return *ptr;
        } else {
            return std::nullopt;
        }
    }
};

inline constexpr as_optional_t as_optional;

// -------------------------------------------------------------------------------------------------

class as_optional_move_t
{
public:
    template<typename T>
    static constexpr std::optional<T> operator()(T *ptr)
    {
        if(ptr) {
            return std::move(*ptr);
        } else {
            return std::nullopt;
        }
    }
};

inline constexpr as_optional_move_t as_optional_move;

// -------------------------------------------------------------------------------------------------

template<typename Like>
class as_optional_like_t
{
public:
    template<typename T>
    static constexpr std::optional<T> operator()(T *ptr)
    {
        if(ptr) {
            return std::forward_like<Like>(*ptr);
        } else {
            return std::nullopt;
        }
    }
};

template<typename Like>
constexpr as_optional_like_t<Like> as_optional_like;

} // namespace siga::util
