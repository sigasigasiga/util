#include <sstream>

#include <grace/experimental/util/utility.hpp>

using namespace grace::experimental::util;

template<forward_ref<std::istream> IstreamFwd>
void foo(IstreamFwd &&is_fwd)
{
    auto &&is = unwrap_forward_ref<std::istream>(std::forward<IstreamFwd>(is_fwd));

    using result_t = decltype(is);
    static_assert(std::same_as<std::remove_cvref_t<result_t>, std::istream>);
}

template<bool IsExplicit>
struct conditional_converter
{
    std::istringstream iss_;
    explicit(IsExplicit) operator std::istream &() { return iss_; }
};

template<bool IsExplicit>
concept test_explicitness =
    requires(conditional_converter<IsExplicit> converter) { foo(converter); };

int main()
{
    std::istringstream iss;
    foo(iss);
    foo(std::istringstream{});

    static_assert(!test_explicitness<false>);
    static_assert(!test_explicitness<true>);
}
