#pragma once

#include <concepts>
#include <string>

namespace siga::util {

template<typename DelimT, typename CharT = char, typename Traits = std::char_traits<CharT>>
requires std::copy_constructible<DelimT> || std::move_constructible<DelimT>
class [[nodiscard]] ostream_joiner
{
public:
    using char_type = CharT;
    using traits_type = Traits;
    using ostream_type = std::basic_ostream<CharT, Traits>;
    using value_type = void;
    using difference_type = std::ptrdiff_t; // cppreference states that this should be `void` but
                                            // `std::ranges` algorithms do not work with that
    using pointer = void;
    using reference = void;
    using iterator_category = std::output_iterator_tag;

public:
    ostream_joiner(ostream_type &os, const DelimT &delim)
        : os_{std::addressof(os)}
        , delim_{delim}
        , first_{true}
    {
    }

    ostream_joiner(ostream_type &os, DelimT &&delim)
        : os_{std::addressof(os)}
        , delim_{std::move(delim)}
        , first_{true}
    {
    }

    ostream_joiner(const ostream_joiner &) = default;
    ostream_joiner(ostream_joiner &&) = default;

    ostream_joiner &operator=(const ostream_joiner &) = default;
    ostream_joiner &operator=(ostream_joiner &&) = default;

public:
    template<typename T>
    ostream_joiner &operator=(const T &value)
    {
        if(!std::exchange(first_, false)) {
            *os_ << delim_;
        }

        *os_ << value;

        return *this;
    }

    ostream_joiner &operator*() noexcept { return *this; }

    ostream_joiner &operator++() noexcept { return *this; }

    ostream_joiner &operator++(int) noexcept { return *this; }

private:
    ostream_type *os_;
    DelimT delim_;
    bool first_;
};

template<typename CharT, typename Traits, typename DelimT>
auto make_ostream_joiner(std::basic_ostream<CharT, Traits> &os, DelimT &&delim)
{
    return ostream_joiner<std::decay_t<DelimT>, CharT, Traits>{os, std::forward<DelimT>(delim)};
}

// -------------------------------------------------------------------------------------------------

template<typename Container>
auto rerase(Container &container, std::reverse_iterator<typename Container::iterator> rit)
{
    ++rit;
    return std::make_reverse_iterator(container.erase(rit.base()));
}

template<typename Container>
auto rerase(
    Container &container,
    std::reverse_iterator<typename Container::iterator> rbegin,
    std::reverse_iterator<typename Container::iterator> rend
)
{
    return std::make_reverse_iterator(container.erase(rend.base(), rbegin.base()));
}

// -------------------------------------------------------------------------------------------------

template<std::input_or_output_iterator It>
[[nodiscard]] constexpr decltype(auto) get_lowest_iter_base(It &&it)
{
    constexpr bool has_base = requires {
        { std::forward<It>(it).base() } -> std::input_or_output_iterator;
    };

    if constexpr(has_base) {
        return get_lowest_iter_base(std::forward<It>(it).base());
    } else {
        return std::forward<It>(it);
    }
}

template<std::ranges::range Range>
[[nodiscard]] constexpr decltype(auto) get_lowest_range_base(Range &&range)
{
    constexpr bool has_base = requires {
        { std::forward<Range>(range).base() } -> std::ranges::range;
    };

    if constexpr(has_base) {
        return get_lowest_range_base(std::forward<Range>(range).base());
    } else {
        return std::forward<Range>(range);
    }
}

} // namespace siga::util
