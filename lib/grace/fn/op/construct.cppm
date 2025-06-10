module;

#include <type_traits>
#include <utility>

export module grace.fn.op:construct;

export namespace grace::fn::op {

template<typename T, bool UseRoundBrackets = true>
class [[nodiscard]] construct
{
public:
    template<typename... Args>
    [[nodiscard]] static constexpr auto operator()(Args &&...args)
        noexcept(noexcept(T(std::forward<Args>(args)...)))
        -> decltype(T(std::forward<Args>(args)...))
    {
        return T(std::forward<Args>(args)...);
    }
};

template<typename T>
class [[nodiscard]] construct<T, false>
{
public:
    template<typename... Args>
    [[nodiscard]] static constexpr auto operator()(Args &&...args)
        noexcept(noexcept(T{std::forward<Args>(args)...}))
        -> decltype(T{std::forward<Args>(args)...})
    {
        return T{std::forward<Args>(args)...};
    }
};

template<typename T>
requires std::is_void_v<T>
class [[nodiscard]] construct<T, true>
{
public:
    static constexpr void operator()() noexcept {}
};

} // namespace grace::fn::op
