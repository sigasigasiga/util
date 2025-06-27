#include <concepts>
#include <utility>

import grace.utility;

int main()
{
    using grace::utility::read_only_value;
    static_assert(std::constructible_from<read_only_value<int>, int>);
    static_assert(std::constructible_from<read_only_value<int>, read_only_value<int>>);
    static_assert(!std::assignable_from<read_only_value<int>, int>);
    static_assert(!std::assignable_from<read_only_value<int>, read_only_value<int>>);

    read_only_value v{777};
    if (v.get() != 777) {
        return 1;
    }

    if (std::move(v).release() != 777) {
        return 2;
    }
}
