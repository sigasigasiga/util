module;

#include <iterator>

export module grace.iterator:rerase;

export namespace grace::iterator {

template<typename Container>
[[nodiscard]] constexpr auto rerase(
    Container &container,
    std::reverse_iterator<typename Container::iterator> rit
)
    noexcept(noexcept(std::make_reverse_iterator(container.erase((++rit).base()))))
    -> decltype(std::make_reverse_iterator(container.erase((++rit).base())))
{
    return std::make_reverse_iterator(container.erase((++rit).base()));
}

template<typename Container>
[[nodiscard]] constexpr auto rerase(
    Container &container,
    std::reverse_iterator<typename Container::iterator> rbegin,
    std::reverse_iterator<typename Container::iterator> rend
)
    noexcept(noexcept(std::make_reverse_iterator(container.erase(rend.base(), rbegin.base()))))
    -> decltype(std::make_reverse_iterator(container.erase(rend.base(), rbegin.base())))
{
    return std::make_reverse_iterator(container.erase(rend.base(), rbegin.base()));
}

template<typename Container>
[[nodiscard]] constexpr auto rerase(
    Container &container,
    std::reverse_iterator<typename Container::const_iterator> rit
)
    noexcept(noexcept(std::make_reverse_iterator(container.erase((++rit).base()))))
    -> decltype(std::make_reverse_iterator(container.erase((++rit).base())))
{
    return std::make_reverse_iterator(container.erase((++rit).base()));
}

template<typename Container>
[[nodiscard]] constexpr auto rerase(
    Container &container,
    std::reverse_iterator<typename Container::const_iterator> rbegin,
    std::reverse_iterator<typename Container::const_iterator> rend
)
    noexcept(noexcept(std::make_reverse_iterator(container.erase(rend.base(), rbegin.base()))))
    -> decltype(std::make_reverse_iterator(container.erase(rend.base(), rbegin.base())))
{
    return std::make_reverse_iterator(container.erase(rend.base(), rbegin.base()));
}

} // namespace grace::iterator
