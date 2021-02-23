#include "PSimple2DPlotAreaCalculator.hpp"
#include <iostream>

namespace physapi
{
    phys_float PSimple2DPlotAreaCalculator::operator()(std::span<const std::pair<phys_float, phys_float>> plot)
    {
        if (plot.empty() || plot.size() <= 1)
        {
            return 0.0;
        }

        phys_float area_tot{};

        for (phys_size i = 0; i < plot.size() - 1; i++)
        {
            phys_float rect_base{plot[i + 1].first - plot[i].first};
            phys_float rect_height{
                (plot[i + 1].second <= plot[i].second)
                    ? plot[i + 1].second
                    : plot[i].second};

            if (rect_base < 0 || rect_height < 0)
            {
                std::cerr << "the rectangle sides cannot be zero!" << std::endl;
                std::cerr << "but rect_base:  " << rect_base << std::endl;
                std::cerr << "but rect_height:  " << rect_height << std::endl;
                std::abort();
            }

            phys_float triangle_height = {std::abs(plot[i + 1].second - plot[i].second)};
            phys_float triangle_leg = std::sqrt(sqr(rect_base) + sqr(triangle_height));
            auto triangle = Triangle{rect_base, triangle_height, triangle_leg};
            auto rect_area = rect_base * rect_height;
            area_tot += (rect_area + triangle.area());
        }

        return area_tot;
    }

    phys_float PSimple2DPlotAreaCalculator::operator()(std::span<const std::pair<phys_float, phys_float>> plot,
                                                       phys_float a, phys_float b)
    {
        if (plot.empty() || plot.size() <= 1)
        {
            return 0.0;
        }

        phys_float area_tot{};

        for (phys_size i = 0; i < plot.size() - 1; i++)
        {
            if (!checkRangeInclusive(a, plot[i].first, b) ||
                !checkRangeInclusive(a, plot[i + 1].first, b))
            {
                continue;
            }

            phys_float rect_base{plot[i + 1].first - plot[i].first};
            phys_float rect_height{
                (plot[i + 1].second <= plot[i].second)
                    ? plot[i + 1].second
                    : plot[i].second};

            if (rect_base < 0 || rect_height < 0)
            {
                std::cerr << "the rectangle sides cannot be zero!" << std::endl;
                std::cerr << "but rect_base:  " << rect_base << std::endl;
                std::cerr << "but rect_height:  " << rect_height << std::endl;
                std::abort();
            }

            phys_float triangle_height = {std::abs(plot[i + 1].second - plot[i].second)};
            phys_float triangle_leg = std::sqrt(sqr(rect_base) + sqr(triangle_height));
            auto triangle = Triangle{rect_base, triangle_height, triangle_leg};
            auto rect_area = rect_base * rect_height;
            area_tot += (rect_area + triangle.area());
        }

        return area_tot;
    }

} // namespace physapi
