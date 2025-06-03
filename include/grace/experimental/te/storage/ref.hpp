#pragma once

#include <memory>

namespace grace::experimental::te::storage {

template<typename T>
class ref
{
public:
    constexpr ref() = default;

    template<typename U>
    constexpr ref(U &v) : data_{std::addressof(v)} {}

public:
    T *get() noexcept { return data_; }
    const T *get() const noexcept { return data_; }

    T &operator*() noexcept { return *get(); }
    const T &operator*() const noexcept { return *get(); }

    T *operator->() noexcept { return get(); }
    const T *operator->() const noexcept { return get(); }

private:
    T *data_;
};

} // namespace grace::experimental::te::storage
