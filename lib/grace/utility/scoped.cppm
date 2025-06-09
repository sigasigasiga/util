export module grace.utility:scoped;

export namespace grace::utility {

class scoped
{
public:
    constexpr scoped() = default;

    constexpr scoped(const scoped &) = delete;
    constexpr scoped &operator=(const scoped &) = delete;

    constexpr scoped(scoped &&) = delete;
    constexpr scoped &operator=(scoped &&) = delete;
};

} // namespace grace::utility
