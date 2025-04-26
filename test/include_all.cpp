#include <grace/compat.hpp>
#include <grace/experimental.hpp>
#include <grace/fn.hpp>
#include <grace/iter.hpp>
#include <grace/meta.hpp>
#include <grace/ranges.hpp>
#include <grace/toy.hpp>
#include <grace/util.hpp>

int main()
{
    using namespace grace;

    util::ignore = algo::find(std::array{1, 2, 3}, 3);
    util::ignore = compat::bind_back(1, 1);
    util::ignore = fn::bind::equal_to(3);
    util::ignore = fn::op::subscript();
    util::ignore = fn::wrap::args::ignore([] {});
    util::ignore = fn::wrap::ret::decay_copy([] { return 1; });
    util::ignore = iter::make_ostream_joiner(std::cout, " ");
    util::ignore = meta::make_tag<0>();
    util::ignore = ranges::make_subrange(std::pair{(int *)0, (int *)0});
    util::ignore = toy::printing_constructor();
    util::ignore = util::read_only_value(3);
}
