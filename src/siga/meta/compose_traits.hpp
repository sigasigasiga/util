#pragma once

#include <siga/meta/apply_traits.hpp>

namespace siga::meta {

template<template<typename...> typename... Traits>
class compose_traits
{
public:
    template<typename T>
    using trait = apply_traits<T, Traits...>;
};

#if 0
template<typename T>
using remove_cvref_and_add_lvalue_ref =
    compose_traits<std::remove_cvref, std::add_lvalue_reference>::trait<T>;

template<typename T>
using remove_cvref_and_add_lvalue_ref_t = remove_cvref_and_add_lvalue_ref<T>::type;

static_assert(std::same_as<remove_cvref_and_add_lvalue_ref_t<const int &&>, int &>);
#endif

} // namespace siga::meta
