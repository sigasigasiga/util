#pragma once

namespace siga::meta {

template<typename T, template<typename...> typename Trait, template<typename...> typename... Rest>
class apply_traits
{
public:
    using type = apply_traits<typename Trait<T>::type, Rest...>::type;
};

template<typename T, template<typename...> typename Trait>
class apply_traits<T, Trait>
{
public:
    using type = Trait<T>::type;
};

template<typename T, template<typename...> typename... Traits>
using apply_traits_t = apply_traits<T, Traits...>::type;

#if 0
static_assert(
    std::same_as<
        apply_traits_t<const int &, std::remove_reference, std::remove_cv>,
        int
    >
);
#endif

} // namespace siga::meta
