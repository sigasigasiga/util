#pragma once

#include <iterator>

namespace siga::iter {

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

} // namespace siga::iter
