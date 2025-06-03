#pragma once

#include <memory>

#include <grace/util/utility.hpp>

namespace grace::experimental::te::storage {

// NB:
// 1. It copies the underlying object on copy
// 2. It is not usable in moved-from state even if the underlying object is
template<typename T>
class heap
{
public:
    constexpr heap() = default;

    constexpr heap(const heap &rhs)
        : clone_{rhs.clone_}
        , data_{clone_(*rhs.data_)}
    {
    }

    constexpr heap &operator=(const heap &rhs)
    {
        return util::default_assign(*this, rhs);
    }

    constexpr heap(heap &&) = default;
    constexpr heap &operator=(heap &&) = default;

    template<typename FwdU, typename U = std::remove_cvref_t<FwdU>>
    requires std::convertible_to<U *, T *>
    constexpr heap_storage(FwdU &&v)
        : clone_{[](const T &v) -> std::unique_ptr<T> { return std::make_unique<U>(static_cast<const U &>(v)); }}
        , data_{std::make_unique<U>(std::forward<FwdU>(v))}
    {
    }

    template<typename FwdU>
    requires std::convertible_to<std::remove_cvref_t<FwdU> *, T *>
    constexpr heap_storage &operator=(FwdU &&v)
    {
        return util::default_assign(*this, std::forward<FwdU>(v));
    }

    constexpr ~heap_storage() = default;

public:
    constexpr void swap(heap &rhs) noexcept
    {
        std::swap(clone_, rhs.clone_);
        std::swap(data_, rhs.data_);
    }

    T *get() noexcept { return data_.get(); }
    const T *get() const noexcept { return data_.get(); }

    T &operator*() noexcept { return *get(); }
    const T &operator*() const noexcept { return *get(); }

    T *operator->() noexcept { return get(); }
    const T *operator->() const noexcept { return get(); }

private:
    std::unique_ptr<T> (*clone_)(const T &);
    std::unique_ptr<T> data_;
};

} // namespace grace::experimental::te::storage
