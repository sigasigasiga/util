#include <siga/ranges/consign_view.hpp>

int main()
{
    constexpr int x = 3;

    // constexpr auto v = std::views::single(x) | siga::ranges::views::consign([] {});
    constexpr siga::ranges::consign_view _(std::views::single(x), 3);
    constexpr auto _ = std::views::single(x) | siga::ranges::views::consign([] {});
    constexpr auto _ = siga::ranges::views::consign([] {})(std::views::single(x));
}
