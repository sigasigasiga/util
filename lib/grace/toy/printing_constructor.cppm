module;

#include <iostream>

export module grace.toy:printing_constructor;

export namespace grace::toy {

class printing_constructor
{
public:
    printing_constructor()
    {
        std::cout << "printing_constructor(): " << this << std::endl;
    }

    printing_constructor(const printing_constructor &rhs)
    {
        std::cout
            << "printing_constructor(const printing_constructor &); "
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
    }

    printing_constructor(printing_constructor &&rhs)
    {
        std::cout
            << "printing_constructor(printing_constructor &&); "
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;
    }

    printing_constructor &operator=(const printing_constructor &rhs)
    {
        std::cout
            << "printing_constructor &operator=(const printing_constructor &); " 
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;

        return *this;
    }

    printing_constructor &operator=(printing_constructor &&rhs)
    {
        std::cout
            << "printing_constructor &operator=(printing_constructor &&); " 
            << "lhs: " << this << ", "
            << "rhs: " << &rhs
            << std::endl;

        return *this;
    }

    ~printing_constructor()
    {
        std::cout << "~printing_constructor(): " << this << std::endl;
    }
};

} // namespace grace::toy
