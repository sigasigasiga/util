#include <siga/util/expected.hpp>

int main()
{
    constexpr std::optional<int> o;
    constexpr auto v = siga::util::ok_or(o, 777);
    static_assert(!v && v.error() == 777);

    constexpr auto v2 = siga::util::ok_or_else(o, [] { return 778; });
    static_assert(!v2 && v2.error() == 778);
}
