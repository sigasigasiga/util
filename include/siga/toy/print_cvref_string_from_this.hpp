#pragma once

#include <iostream>

#include <siga/toy/cvref_string_from_args.hpp>

namespace siga::toy {

class [[nodiscard]] print_cvref_string_from_this
{
public:
    template<typename Self>
    void operator()(this Self &&self)
    {
        std::cout << cvref_string_from_args{}(std::forward<Self>(self)) << std::endl;
    }
};

} // namespace siga::toy
