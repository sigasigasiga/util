module;

#include <concepts>

export module grace.concepts:without_cvref;

export namespace grace::concepts {

template<typename T>
concept without_cvref = std::same_as<std::remove_cvref_t<T>, T>;

} // namespace grace::concepts
