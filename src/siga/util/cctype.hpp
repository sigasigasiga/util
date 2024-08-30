#pragma once

#include <cctype>

namespace siga::util {

// clang-format off

inline bool is_alnum(unsigned char c) { return std::isalnum(c); }
inline bool is_alpha(unsigned char c) { return std::isalpha(c); }
inline bool is_blank(unsigned char c) { return std::isblank(c); }
inline bool is_cntrl(unsigned char c) { return std::iscntrl(c); }
inline bool is_digit(unsigned char c) { return std::isdigit(c); }
inline bool is_graph(unsigned char c) { return std::isgraph(c); }
inline bool is_lower(unsigned char c) { return std::islower(c); }
inline bool is_print(unsigned char c) { return std::isprint(c); }
inline bool is_punct(unsigned char c) { return std::ispunct(c); }
inline bool is_space(unsigned char c) { return std::isspace(c); }
inline bool is_upper(unsigned char c) { return std::isupper(c); }
inline bool is_xdigit(unsigned char c) { return std::isxdigit(c); }
inline bool to_lower(unsigned char c) { return std::tolower(c); }
inline bool to_upper(unsigned char c) { return std::toupper(c); }

// clang-format on

} // namespace siga::util
