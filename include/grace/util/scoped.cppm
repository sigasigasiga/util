export module grace.util.scoped;

export namespace grace::util {

class scoped
{
public:
    constexpr scoped() = default;

    constexpr scoped(const scoped &) = delete;
    constexpr scoped &operator=(const scoped &) = delete;

    constexpr scoped(scoped &&) = delete;
    constexpr scoped &operator=(scoped &&) = delete;
};

class move_only
{
public:
    constexpr move_only() = default;

    constexpr move_only(const move_only &) = delete;
    constexpr move_only &operator=(const move_only &) = delete;

    constexpr move_only(move_only &&) = default;
    constexpr move_only &operator=(move_only &&) = default;
};

} // namespace grace::util
