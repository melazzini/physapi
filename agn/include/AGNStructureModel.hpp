#pragma once

#include "agn_utils.hpp"

namespace agn
{
    /**
     * @brief This class represents an abstract AGN structure.
     * 
     * This is the base class for all the agn structures, smooth or clumpy,
     * toroidal, spherical, etc.
     * 
     * @ingroup agn
     * 
     */
    class AGNStructureModel
    {
    public:
        /**
         * @brief Get the distance to the next boundary inside the agn.
         * 
         * @param position reference point
         * @param direction direction of the trajectory
         * @return phys_float distance to the next boundary
         * @return -1   if there is no boundary for the given direction and reference point. 
         */
        virtual phys_float distanceToBoundary(const PPosition &position, const PVector3D &direction) const = 0;

        /**
         * @brief Get the distance to the interaction point.
         * 
         * @param position reference point
         * @param direction direction of the trajectory
         * @param lambdaMean mean free path
         * @param escapeProbability escape probability
         * @param rand_0_1 random value between 0 and 1
         * @return phys_float  distance to the interaction point
         * @return -1 there is no interaction point
         */
        virtual phys_float distanceToInteractionPoint(const PPosition &position,
                                                      const PVector3D &direction,
                                                      phys_float lambdaMean,
                                                      phys_float escapeProbability,
                                                      phys_float rand_0_1) const
        {
            return -lambdaMean * std::log(1 - rand_0_1 * (1 - escapeProbability));
        }

        /**
         * @brief Get the distance to the next entering point of the agn structure
         * 
         * @param position reference point
         * @param direction direction of the trajectory
         * @return phys_float distance to the entering point
         * @return -1 there is not entering point for the given arguments
         */
        virtual phys_float distanceToEnteringPoint(const PPosition &position,
                                                   const PVector3D &direction) const = 0;

        /**
         * @brief Get the equatorial effective half size of the agn structure model
         * 
         * @return phys_float 
         */
        virtual phys_float equatorialEffectiveHalfSize() const = 0;

        /**
         * @brief Returns true if the given point is inside the structure, 
         * otherwise false
         * 
         * @param position point position
         * @return phys_bool 
         */
        virtual phys_bool isInside(const PPosition &position) const = 0;

        /**
         * @brief Get the concentration of hydrogen
         * 
         * @return phys_float 
         */
        virtual phys_float n_H() const = 0;
    };
} // namespace agn
