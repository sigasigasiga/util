#include <grace/ranges/consign_view.hpp>

int main()
{
    constexpr int x = 3;

    // constexpr auto v = std::views::single(x) | grace::ranges::views::consign([] {});
    constexpr grace::ranges::consign_view _(std::views::single(x), 3);
    constexpr auto _ = std::views::single(x) | grace::ranges::views::consign([] {});
    constexpr auto _ = grace::ranges::views::consign([] {})(std::views::single(x));
}
