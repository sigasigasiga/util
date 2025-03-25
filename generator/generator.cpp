#include <cassert>
#include <expected>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>

#include <siga/iter/ostream_joiner.hpp>
#include <siga/util/exception_storage.hpp>

template<typename T>
struct [[nodiscard]] result : std::expected<T, siga::util::exception_storage<std::exception>>
{
    using std::expected<T, siga::util::exception_storage<std::exception>>::expected;

    template<typename Self>
    decltype(auto) unwrap(this Self &&self)
    {
        if(self.has_value()) {
            return *std::forward<Self>(self);
        } else {
            std::forward<Self>(self).error().throw_exception();
        }
    }

private:
    using std::expected<T, siga::util::exception_storage<std::exception>>::value;
};

template<typename Ex, typename... Args>
auto make_error(Args &&...args)
{
    return std::unexpected(siga::util::exception_storage(Ex(std::forward<Args>(args)...)));
}

std::string make_include_directive(const std::filesystem::path &hpp_path)
{
    return std::format("#include <{}>", hpp_path.generic_string());
}

result<void>
write_header(const std::filesystem::path &canonical_dir_path, std::output_iterator<char> auto oit)
{
    if(canonical_dir_path.empty()) {
        return make_error<std::runtime_error>("the path is empty");
    }

    if(canonical_dir_path.native().back() == std::filesystem::path::value_type('.')) {
        return make_error<std::runtime_error>("the path ends with `.`");
    }

    // clang-format off
    auto includes = std::filesystem::directory_iterator{canonical_dir_path}
        | std::views::filter([](auto &&x) { return x.path().extension() == ".hpp"; })
        | std::views::filter([](auto &&p) { return p.is_regular_file(); })
        | std::views::transform(make_include_directive)
        | std::views::join_with('\n')
        ;
    // clang-format on

    std::ranges::copy(includes, oit);
    return {};
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
            std::cout << "creating " << base << std::endl;
            std::filesystem::create_directories(base.parent_path());
        }

        std::ofstream header_ofs{base += ".hpp", std::ios_base::out | std::ios_base::trunc};

        write_header(dir, std::ostream_iterator<char>(header_ofs)).unwrap();
    }
}
