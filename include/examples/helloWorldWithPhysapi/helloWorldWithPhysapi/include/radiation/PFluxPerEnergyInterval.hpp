#pragma once

#include "radiation_utils.hpp"
#include <iostream>

namespace physapi
{
    /**
     * @brief This class represents flux per energy interval.
     * 
     * @tparam energyIntervals number of energy intervals.
     * 
     * @see PFluxPerEnergyIntervalMaker
     * 
     * @ingroup radiation
     * 
     */
    template <phys_size energyIntervals>
    class PFluxPerEnergyInterval
    {
    protected:
        PFluxPerEnergyInterval() {}

    public:
        /**
         * @brief Destroy the PFluxPerEnergyInterval object
         * 
         */
        virtual ~PFluxPerEnergyInterval() {}

        template <phys_size energyIntervals_>
        friend std::ostream &operator<<(std::ostream &os, const PFluxPerEnergyInterval<energyIntervals_> &flux_dE);

        template <phys_size energyIntervals_>
        friend class PFluxPerEnergyIntervalMaker;

    private:
        // this is the internal container with
        // the flux per energy interval data
        t_spectrum<energyIntervals> m_data;
    };

    template <phys_size energyIntervals_>
    std::ostream &operator<<(std::ostream &os, const PFluxPerEnergyInterval<energyIntervals_> &flux_dE)
    {
        for (size_t i = 0; i < flux_dE.m_data.size(); i++)
        {
            os << flux_dE.m_data[i].first << "   "
               << flux_dE.m_data[i].second;
            if ((i + 1) < flux_dE.m_data.size())
            {
                os << std::endl;
            }
        }

        return os;
    }

} // namespace physapi
