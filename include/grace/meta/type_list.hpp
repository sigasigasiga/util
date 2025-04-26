#pragma once

namespace grace::meta {

template<typename... Ts>
class [[nodiscard]] type_list
{
public:
    template<typename T>
    using push_back = type_list<Ts..., T>;
};

} // namespace grace::meta
