#pragma once

#include <cstdlib>

namespace siga::fn::op {

class free
{
public:
    // TODO: `noexcept`?
    static void operator()(void *ptr) { std::free(ptr); }
};

} // namespace siga::fn::op
