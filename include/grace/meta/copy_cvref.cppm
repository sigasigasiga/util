module;

#include <type_traits>

export module grace.meta.copy_cvref;

export namespace grace::meta {

template<typename From, typename To>
class copy_const
{
public:
    // TODO: should we remove `const` from `To` if `From` doesn't have it?
    using type = To;
};

template<typename From, typename To>
class copy_const<const From, To>
{
public:
    using type = const To;
};

template<typename From, typename To>
using copy_const_t = copy_const<From, To>::type;

// -------------------------------------------------------------------------------------------------

template<typename From, typename To>
class copy_volatile
{
public:
    using type = To;
};

template<typename From, typename To>
class copy_volatile<volatile From, To>
{
public:
    using type = volatile To;
};

template<typename From, typename To>
using copy_volatile_t = copy_volatile<From, To>::type;

// -------------------------------------------------------------------------------------------------

template<typename From, typename To>
class copy_cv
{
public:
    using type = copy_const_t<From, copy_volatile_t<From, To>>;
};

template<typename From, typename To>
using copy_cv_t = copy_cv<From, To>::type;

// -------------------------------------------------------------------------------------------------

template<typename From, typename To>
class copy_lvalue_ref
{
public:
    using type = To;
};

template<typename From, typename To>
class copy_lvalue_ref<From &, To>
{
public:
    using type = To &;
};

template<typename From, typename To>
using copy_lvalue_ref_t = copy_lvalue_ref<From, To>::type;

// -------------------------------------------------------------------------------------------------

template<typename From, typename To>
class copy_rvalue_ref
{
public:
    using type = To;
};

// TODO: this class won't copy the rvalue reference if `To` is an lvalue reference type.
//       whether it is okay or not is a philosophical question
template<typename From, typename To>
class copy_rvalue_ref<From &&, To>
{
public:
    using type = To &&;
};

template<typename From, typename To>
using copy_rvalue_ref_t = copy_rvalue_ref<From, To>::type;

// -------------------------------------------------------------------------------------------------

template<typename From, typename To>
class copy_ref
{
public:
    using type = copy_lvalue_ref_t<From, copy_rvalue_ref_t<From, To>>;
};

template<typename From, typename To>
using copy_ref_t = copy_ref<From, To>::type;

// -------------------------------------------------------------------------------------------------

template<typename From, typename To>
class copy_cvref
{
private:
    using from_unref_t = std::remove_reference_t<From>;
    using to_unref_t = std::remove_reference_t<To>;
    using to_cv_t = copy_cv_t<from_unref_t, to_unref_t>;
    using to_cv_ref_t = copy_ref_t<From, to_cv_t>;

public:
    using type = to_cv_ref_t;
};

template<typename From, typename To>
using copy_cvref_t = copy_cvref<From, To>::type;

} // namespace grace::meta
