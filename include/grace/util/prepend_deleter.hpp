#pragma once

#include <grace/fn/bind/sequence.hpp>
#include <grace/util/get_deleter.hpp>

namespace grace::util {

// NB: if `D`'s move constructor throws, the program is terminated.
//     `std::unique_ptr`'s constructor has the same behavior
template<typename T, typename D, typename F>
constexpr auto prepend_deleter(std::unique_ptr<T, D> p, F &&fn) noexcept
    -> decltype(std::unique_ptr<
        T,
        decltype(fn::bind::sequence(std::forward<F>(fn), (get_deleter)(std::move(p))))
    >{
        p.release(),
        fn::bind::sequence(std::forward<F>(fn), (get_deleter)(std::move(p)))
    })
{
    return std::unique_ptr<
        T,
        decltype(fn::bind::sequence(std::forward<F>(fn), (get_deleter)(std::move(p))))
    >{
        p.release(),
        fn::bind::sequence(std::forward<F>(fn), (get_deleter)(std::move(p)))
    };
}

} // namespace grace::util
