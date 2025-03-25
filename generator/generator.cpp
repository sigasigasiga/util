#include <cassert>
#include <expected>
#include <filesystem>
#include <fstream>
#include <map>
#include <ranges>
#include <vector>

std::string make_include_directive(const std::filesystem::path &hpp_path)
{
    return std::format("#include <{}>", hpp_path.generic_string());
}

template<std::ranges::input_range DirEntRange>
requires std::same_as<std::filesystem::directory_entry, std::ranges::range_value_t<DirEntRange>>
auto make_header_content(DirEntRange &&entries)
{
    // clang-format off
    return std::forward<DirEntRange>(entries)
        | std::views::filter([](auto &&x) { return x.path().extension() == ".hpp"; })
        | std::views::filter([](auto &&p) { return p.is_regular_file(); })
        | std::views::transform(make_include_directive)
        | std::views::join_with('\n')
        ;
    // clang-format on
}

int main()
{
    const auto input_path = std::filesystem::canonical("../include/");

    std::map<int, std::vector<std::filesystem::path>, std::ranges::greater> dirs;

    for(std::filesystem::recursive_directory_iterator it{input_path}, end{}; it != end; ++it) {
        if(it->is_directory()) {
            dirs[it.depth()].push_back(*it); // TODO: should we call `canonical` again?
        }
    }

    for(const auto &dir : dirs | std::views::values | std::views::join) {
        auto base = std::filesystem::relative(dir, input_path);
        if(auto parent = base.parent_path(); !parent.empty()) {
            std::filesystem::create_directories(base.parent_path());
        }

        assert(!dir.empty());

        std::ofstream header_ofs{base += ".hpp", std::ios_base::out | std::ios_base::trunc};

        std::ranges::copy(
            make_header_content(std::filesystem::directory_iterator{dir}),
            std::ostream_iterator<char>(header_ofs)
        );
    }
}
