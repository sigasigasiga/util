#pragma once

#include <memory>

#include <grace/util/utility.hpp>

namespace grace::iter::pm {

template<typename T>
class output_iterator
{
public:
    class impl
    {
    public:
        virtual ~impl() = default;

    public:
        virtual void next() = 0;
        virtual impl *clone() = 0;
        virtual void assign(const T &value) = 0;
        virtual void assign(T &&value) = 0;
    };

public:
    using difference_type = std::ptrdiff_t;

public:
    constexpr output_iterator(std::unique_ptr<impl> impl) : impl_{std::move(impl)} {}

    constexpr output_iterator() = default;
    constexpr output_iterator(const output_iterator &rhs) : impl_{rhs.impl_->clone()} {}
    constexpr output_iterator(output_iterator &&) = default;
    constexpr output_iterator &operator=(const output_iterator &rhs) { return grace::util::default_assign(*this, rhs); }
    constexpr output_iterator &operator=(output_iterator &&) = default;

    constexpr void swap(output_iterator &rhs) noexcept { std::swap(impl_, rhs.impl_); }

public:
    output_iterator &operator*() noexcept { return *this; }
    output_iterator &operator++() { impl_->next(); return *this; }
    output_iterator operator++(int) { auto copy = *this; impl_->next(); return copy; }

    output_iterator &operator=(const T &value) { impl_->assign(value); return *this; }
    output_iterator &operator=(T &&value) { impl_->assign(std::move(value)); return *this; }

private:
    std::unique_ptr<impl> impl_;
};

} // namespace grace::iter::pm
