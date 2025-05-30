#pragma once

#include <memory>

#include <grace/meta/copy_cvref.hpp>

namespace grace::util {

namespace detail_get_deleter {

template<typename T>
struct unique_ptr_kind;

template<typename T, typename D>
struct unique_ptr_kind<std::unique_ptr<T, D>>
{
    using regular = void;
};

template<typename T, typename D>
struct unique_ptr_kind<std::unique_ptr<T, D &>>
{
    using ref_del = void;
};

} // namespace detail_get_deleter

// allowing `deleter_type` to be a reference was a mistake
template<typename FwdPtr, typename Ptr = std::remove_cvref_t<FwdPtr>>
requires requires { typename detail_get_deleter::unique_ptr_kind<Ptr>::ref_del; }
[[nodiscard]] constexpr auto get_deleter(FwdPtr &&ptr)
    noexcept(noexcept(std::ref(ptr.get_deleter())))
    -> decltype(std::ref(ptr.get_deleter()))
{
    return std::ref(ptr.get_deleter());
}

// not having rvalue overloads for `get_deleter` was a mistake
template<
    typename FwdPtr,
    typename Ptr = std::remove_cvref_t<FwdPtr>,
    typename Del = Ptr::deleter_type,
    typename FwdDel = grace::meta::copy_cvref_t<FwdPtr &&, Del>>
requires requires { typename detail_get_deleter::unique_ptr_kind<Ptr>::regular; }
[[nodiscard]] constexpr auto get_deleter(FwdPtr &&ptr)
    noexcept(noexcept(static_cast<FwdDel>(ptr.get_deleter())))
    -> decltype(static_cast<FwdDel>(ptr.get_deleter()))
{
    return static_cast<FwdDel>(ptr.get_deleter());
}

} // namespace grace::util
