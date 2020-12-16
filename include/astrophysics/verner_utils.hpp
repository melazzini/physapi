#pragma once
#include "utils.hpp"

namespace physapi
{
    /**
     * @brief This operator is needed to normalize the value
     * of the sigma cross section given by Prof. Verner's algorithm
     * 
     */
    constexpr phys_lfloat operator"" _Mb(phys_lfloat value)
    {
        return value * 1.E-18;
    }
} // namespace physapi
