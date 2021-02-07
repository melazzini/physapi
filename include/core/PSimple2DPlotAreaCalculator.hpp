#pragma once

#include "utils.hpp"
#include <span>

namespace physapi
{
    /**
     * @brief This is a functor class to calculate the area below a curve(scattered),
     * given by a set of points-coordinates.
     * 
     * @ingroup core
     */
    class PSimple2DPlotAreaCalculator
    {
        struct Triangle
        {
            phys_float a, b, c; // legs lengths

            phys_float perimeter() { return a + b + c; }
            phys_float area()
            {
                auto s{perimeter() / 2};
                return std::sqrt(s * (s - a) * (s - b) * (s - c));
            }
        };

    public:
        /**
         * @brief Get the scattered plot's area
         * 
         * @param plot scattered plot(coordinates)
         * @return phys_float area
         */
        phys_float operator()(std::span<const std::pair<phys_float, phys_float>> plot);
    };
} // namespace physapi
