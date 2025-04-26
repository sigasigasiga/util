#pragma once

#include <iostream>

#include <grace/toy/cvref_string_from_args.hpp>

namespace grace::toy {

class [[nodiscard]] print_cvref_string_from_this
{
public:
    template<typename Self>
    void operator()(this Self &&self)
    {
        std::cout << cvref_string_from_args{}(std::forward<Self>(self)) << std::endl;
    }
};

} // namespace grace::toy
