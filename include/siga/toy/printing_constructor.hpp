#pragma once

#include <iostream>

namespace siga::toy {

class printing_constructor
{
public:
    printing_constructor() { std::cout << "printing_constructor(): " << this << std::endl; }

    printing_constructor(const printing_constructor &rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor(const printing_constructor &); "
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on
    }

    printing_constructor(printing_constructor &&rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor(printing_constructor &&); "
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on
    }

    printing_constructor &operator=(const printing_constructor &rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor &operator=(const printing_constructor &); " 
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on

        return *this;
    }

    printing_constructor &operator=(printing_constructor &&rhs)
    {
        // clang-format off
        std::cout
            << "printing_constructor &operator=(printing_constructor &&); " 
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
        // clang-format on

        return *this;
    }

    ~printing_constructor() { std::cout << "~printing_constructor(): " << this << std::endl; }
};

} // namespace siga::toy
