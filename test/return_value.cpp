#include <siga/util/functional.hpp>

int main()
{
    using namespace siga::util;

    auto simple = make_return_value(3);
    using simple_t = decltype(simple);
    static_assert(std::same_as<simple_t, return_value<int>>);
    using simple_ret_t = decltype(simple());
    static_assert(std::same_as<simple_ret_t, int>);

    int x = 0;

    auto manual_return_type_ref = make_return_value<float>(x);
    using manual_return_type_ref_t = decltype(manual_return_type_ref);
    static_assert(std::same_as<manual_return_type_ref_t, return_value<int, float>>);
    using manual_return_type_ref_ret_t = decltype(manual_return_type_ref());
    static_assert(std::same_as<manual_return_type_ref_ret_t, float>);

    auto with_traits = make_return_value<std::unwrap_reference>(std::ref(x));
    using with_traits_t = decltype(with_traits);
    static_assert(std::same_as<with_traits_t, return_value<std::reference_wrapper<int>, int &>>);

    auto return_void = make_return_value<void>();
    using return_void_t = decltype(return_void);
    static_assert(std::same_as<return_void_t, return_value<void>>);
}
