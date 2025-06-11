module;

#include <concepts>
#include <memory>

export module grace.memory:to_address_arr;

import :to_address;

import grace.meta;

namespace {

template<typename Ptr>
constexpr auto impl(const Ptr &ptr, grace::meta::overload_priority<0>)
requires requires(std::size_t i) {
    // overload for `unique_ptr<T[]>` and `shared_ptr<T[]>`.
    // it isn't perfect by any means but i guess that's something?
    { ptr[i] } -> std::same_as<typename std::pointer_traits<Ptr>::element_type &>;
    { ptr.get() } -> std::same_as<typename std::pointer_traits<Ptr>::element_type *>;
}
{
    return ptr.get();
}

template<typename Ptr>
constexpr auto impl(const Ptr &ptr, grace::meta::overload_priority<1> = {})
    noexcept(noexcept(grace::memory::to_address(ptr)))
    -> decltype(grace::memory::to_address(ptr))
{
    return grace::memory::to_address(ptr);
}

} // anonymous namespace

export namespace grace::memory {

// same as `grace::memory::to_address` but supports `{unique,shared}_ptr<T[]>`
[[nodiscard]] constexpr auto to_address_arr(const auto &ptr)
    noexcept(noexcept(impl(ptr)))
    -> decltype(impl(ptr))
{
    return impl(ptr);
}

} // namespace grace::memory
