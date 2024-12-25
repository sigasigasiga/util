#pragma once

namespace siga::fn::bind {

template<typename... Ts>
class [[nodiscard]] overload : public Ts...
{
public:
    using Ts::operator()...;
};

} // namespace siga::fn::bind
