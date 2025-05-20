#pragma once

#include <memory>

#include <grace/util/utility.hpp>

namespace grace::iter::pm {

template<typename ValueType, typename ReferenceType = ValueType>
class input_iterator
{
public:
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;

public:
    class impl
    {
    public:
        virtual ~impl() = default;

    public:
        virtual void next() = 0;
        virtual impl *clone() = 0; // TODO: remove when `Storage`
        virtual ReferenceType get() const = 0;
    };

public:
    constexpr input_iterator(std::unique_ptr<impl> impl) : impl_{std::move(impl)} {}

    constexpr input_iterator() = default;
    constexpr input_iterator(const input_iterator &rhs) : impl_{rhs.impl_->clone()} {}
    constexpr input_iterator(input_iterator &&) = default;
    constexpr input_iterator &operator=(const input_iterator &rhs) { return util::default_assign(*this, rhs); }
    constexpr input_iterator &operator=(input_iterator &&) = default;

    constexpr void swap(input_iterator &rhs) noexcept { std::swap(impl_, rhs.impl_); }

public:
    [[nodiscard]] constexpr ReferenceType operator*() const { return impl_->get(); }
    constexpr input_iterator &operator++() { impl_->next(); return *this; }
    constexpr void operator++(int) { ++*this; }

private:
    std::unique_ptr<impl> impl_; // TODO: we should have an abstraction called `Storage` that'd allow us to have `fast_pimpl`
};

} // namespace grace::iter::pm
