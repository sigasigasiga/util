#pragma once

#include <cstdlib>

namespace siga::fn::op {

class free
{
public:
    static void operator()(void *ptr) noexcept { std::free(ptr); }
};

} // namespace siga::fn::op
