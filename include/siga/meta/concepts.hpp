#pragma once

#include <concepts>

namespace siga::meta {

// idea from Ed Catmur. usage example: `conceptify<std::is_enum>`
// the only drawback is that it's impossible to have NTTPs
template<typename T, template<typename...> typename Trait>
concept conceptify = Trait<T>::value;

// -------------------------------------------------------------------------------------------------

template<typename T>
concept without_cvref = std::same_as<std::remove_cvref_t<T>, T>;

} // namespace siga::meta
