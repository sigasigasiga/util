#pragma once

#include <functional>
#include <memory>
#include <utility>

#include <grace/meta/copy_cvref.hpp>

namespace grace::experimental::te {

namespace detail_poly {

template<typename>
class interface;

template<
    typename Ret,
    typename... Args,
    bool Noexcept>
class interface<Ret (Args...) noexcept(Noexcept)>
{
public:
    virtual ~interface() = default;
    virtual Ret operator()(Args ...args) const noexcept(Noexcept) = 0;
};

template<auto>
class impl;

template<
    typename Ret,
    typename... Args,
    bool Noexcept,
    Ret (*Fn)(Args...) noexcept(Noexcept)>
class impl<Fn> : public interface<Ret (Args...) noexcept(Noexcept)>
{
public:
    virtual Ret operator()(Args ...args) const noexcept(Noexcept) final
    { return Fn(std::forward<Args>(args)...); }
};

template<typename>
struct is_noexcept_fn_ptr : std::false_type {};

template<typename Ret, typename ...Args, bool Noexcept>
struct is_noexcept_fn_ptr<Ret (*)(Args ...) noexcept(Noexcept)> : std::bool_constant<Noexcept> {};

} // namespace detail_poly

template<auto Fn>
class poly
{
private:
    struct base { virtual ~base() = default; };

private:
    using lambda_t = decltype(Fn);
    using fptr_t = decltype(&lambda_t::template operator()<base>);
    using sig_t = std::remove_pointer_t<fptr_t>;

private:
    std::unique_ptr<base> data_;
    detail_poly::interface<sig_t> *fn_;

public:
    poly() = default;

    template<typename T>
    poly(T obj)
    {
        struct der : base {
            der(T &&obj) : obj(std::move(obj)) {}
            T obj;
        };

        // data_
        data_ = std::make_unique<der>(std::move(obj));

        // fn_
        constexpr auto l =
            []<
                typename Base,
                typename... Args
            >
            (Base base, Args ...args)
            static
            noexcept(detail_poly::is_noexcept_fn_ptr<fptr_t>::value)
        {
            using real_type_t = grace::meta::copy_cvref_t<Base, der>;
            return Fn(static_cast<real_type_t>(base).obj, std::forward<Args>(args)...);
        };

        static detail_poly::impl<static_cast<fptr_t>(l)> i;
        fn_ = &i;
    }

public:
    template<typename... Args>
    auto invoke(Args &&...args)
        noexcept(noexcept(std::invoke(*fn_, *data_, std::forward<Args>(args)...)))
        -> decltype(std::invoke(*fn_, *data_, std::forward<Args>(args)...))
    {
        return std::invoke(*fn_, *data_, std::forward<Args>(args)...);
    }
};

} // namespace grace::experimental::te
