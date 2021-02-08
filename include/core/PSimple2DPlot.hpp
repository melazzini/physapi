#pragma once
#include "utils.hpp"
// #include <vector>
namespace physapi
{
    class PSimple2DPlot : public std::vector<std::pair<phys_float, phys_float>>
    {
    public:
        void loadElement(const std::array<phys_float, 2> &a)
        {
            this->push_back({a[0], a[1]});
        }
    };
} // namespace physapi
