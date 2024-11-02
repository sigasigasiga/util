#pragma once

#include <siga/util/storage_base.hpp>

namespace siga::util {

// In Rust mutability is not part of a type but a part of a variable.
// That's an attempt to do something similar to that.
//
// Why `read_only_value` may be better than `const` in some scenarios:
// 1. It is move-constructible and move-assignable
// 2. Like in Rust, you can make the value mutable by moving it to a mutable variable
//
//    Rust: `let x = "".to_string(); let mut y = x;`
//    C++: `auto x = read_only_value(std::string()); auto y = std::move(x).release();`
//
//    In both examples `x` is moved-from and `y` is a new mutable variable
template<typename T>
class read_only_value : private storage_base<T>
{
public:
    template<typename... Args>
    requires std::constructible_from<storage_base<T>, Args...>
    constexpr read_only_value(Args &&...args)
        noexcept(std::is_nothrow_constructible_v<storage_base<T>, Args...>)
        : storage_base<T>(std::forward<Args>(args)...)
    {
    }

    constexpr read_only_value(const read_only_value &) = default;
    constexpr read_only_value(read_only_value &&) = default;

    constexpr read_only_value &operator=(const read_only_value &) = delete;
    constexpr read_only_value &operator=(read_only_value &&) = delete;

public:
    [[nodiscard]] constexpr const T &get() const noexcept { return storage_base<T>::value(); }

    // I'm not sure if allowing `release` only for rvalues is a good idea, but it looks nice to me:
    // this way it'd be easier to notice that the value would be moved-from after the operation.
    //
    // However, it's not consistent with STL -- `unique_ptr` also has lvalue release
    [[nodiscard]] constexpr T &&release() && noexcept { return std::move(*this).value(); }
};

template<typename T>
read_only_value(T) -> read_only_value<T>;

} // namespace siga::util
