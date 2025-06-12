#include <memory>

import grace.memory;

int main() {
    using namespace grace::memory;

    auto p = std::make_unique<int>();

    // TODO: test that it is acutally invoked
    auto p2 = prepend_deleter(std::move(p), [](int *){});
}
