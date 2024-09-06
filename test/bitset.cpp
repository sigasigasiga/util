#include <siga/util/bitset.hpp>

int main()
{
    constexpr auto b = siga::util::make_bitset(1, 1, 0, 0, 1);
    static_assert(b[0] == 1);
    static_assert(b[1] == 1);
    static_assert(b[2] == 0);
    static_assert(b[3] == 0);
    static_assert(b[4] == 1);
}
