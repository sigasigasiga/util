#pragma once

namespace grace::fn::bind {

template<typename... Ts>
class [[nodiscard]] overload : public Ts...
{
public:
    using Ts::operator()...;
};

} // namespace grace::fn::bind
