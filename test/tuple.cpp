#include <ranges>
#include <tuple>

#include <siga/meta/tuple.hpp>

namespace test {

struct foo
{
    template<std::size_t I>
    requires(I == 1)
    int get()
    {
        return 0;
    }
};

template<std::size_t I>
requires(I == 0)
int get(foo)
{
    return 1;
}

} // namespace test

template<std::size_t I>
class std::tuple_element<I, test::foo> : public std::type_identity<int>
{};

template<>
class std::tuple_size<test::foo> : public std::integral_constant<std::size_t, 2>
{};

int main()
{
    static_assert(!siga::meta::tuple_like<test::foo>);
    static_assert(siga::meta::tuple_like<std::tuple<>>);
    static_assert(siga::meta::tuple_like<std::tuple<int, int>>);
    static_assert(siga::meta::tuple_like<std::tuple<int &, int &&>>);
    static_assert(siga::meta::tuple_like<std::ranges::subrange<int *, void *>>);
}
