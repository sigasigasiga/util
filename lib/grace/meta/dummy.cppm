export module grace.meta:dummy;

export namespace grace::meta {

// This class can be used to differentiate between
// a templated constructor that takes a single argument and a copy/move constructor
// by providing a second argument of type `dummy_t`.
//
// libstdc++ uses a `int` for such purposes https://stackoverflow.com/a/50240909/10961484
// but I think that a dummy struct would be more verbose and readable for such purposes.
struct dummy_t {};
inline constexpr dummy_t dummy;

} // namespace grace::meta
