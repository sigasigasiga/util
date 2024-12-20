#include <siga/util/functional.hpp>
#include <siga/util/meta.hpp>

template<typename T>
using ret_t = std::invoke_result_t<T>;

int main()
{
    using namespace siga::util;

    // clang-format off
    using simple_t = return_value<int>;
    static_assert(
        std::same_as<ret_t<simple_t &>, int> &&
        std::same_as<ret_t<const simple_t &>, int> &&
        std::same_as<ret_t<volatile simple_t &>, int> &&
        std::same_as<ret_t<const volatile simple_t &>, int> &&

        std::same_as<ret_t<simple_t &&>, int> &&
        std::same_as<ret_t<const simple_t &&>, int> &&
        std::same_as<ret_t<volatile simple_t &&>, int> &&
        std::same_as<ret_t<const volatile simple_t &&>, int> &&

        true
    );
    // clang-format on

    // clang-format off
    using refwrp_t = return_value<std::reference_wrapper<int>>;
    static_assert(
        std::same_as<ret_t<refwrp_t &>, std::reference_wrapper<int>> &&
        std::same_as<ret_t<const refwrp_t &>, std::reference_wrapper<int>> &&

        std::same_as<ret_t<refwrp_t &&>, std::reference_wrapper<int>> &&
        std::same_as<ret_t<const refwrp_t &&>, std::reference_wrapper<int>> &&

        true
    );
    // clang-format on

    // clang-format off
    using refunwrp_t = return_value<
        std::reference_wrapper<int>,
        compose_traits<std::remove_cvref, std::unwrap_reference>::trait
    >;
    static_assert(
        std::same_as<ret_t<refunwrp_t &>, int &> &&
        std::same_as<ret_t<const refunwrp_t &>, int &> &&

        std::same_as<ret_t<refunwrp_t &&>, int &> &&
        std::same_as<ret_t<const refunwrp_t &&>, int &> &&

        true
    );
    // clang-format on

    // clang-format off
    using fwd_t = return_value<int, std::type_identity>;
    static_assert(
        std::same_as<ret_t<fwd_t &>, int &> &&
        std::same_as<ret_t<const fwd_t &>, const int &> &&

        std::same_as<ret_t<fwd_t &&>, int &&> &&
        std::same_as<ret_t<const fwd_t &&>, const int &&> &&

        true
    );

    // clang-format on
}
