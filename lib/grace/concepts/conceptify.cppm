export module grace.concepts:conceptify;

export namespace grace::concepts {

// idea from Ed Catmur. usage example: `conceptify<std::is_enum>`
// the only drawback is that it's impossible to have NTTPs
template<typename T, template<typename...> typename Trait>
concept conceptify = Trait<T>::value;

} // namespace grace::concepts
