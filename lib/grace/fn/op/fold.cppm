module;

#include <utility>

export module grace.fn.op:fold;

import grace.fn.invoke;

export namespace grace::fn::op {

class [[nodiscard]] fold
{
public:
    template<typename FnTuple, typename... Args>
    static constexpr auto operator()(FnTuple &&tup, Args &&...args)
        noexcept(noexcept(invoke::fold(std::forward<FnTuple>(tup), std::forward<Args>(args)...)))
        -> decltype(invoke::fold( std::forward<FnTuple>(tup), std::forward<Args>(args)...))
    {
        return invoke::fold(std::forward<FnTuple>(tup), std::forward<Args>(args)...);
    }
};

} // namespace grace::fn::op
