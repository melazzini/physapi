#pragma once

#include "AGNStructureModel.hpp"

namespace agn
{
    /**
     * @brief This class represents a smooth torus agn structure
     * 
     * @ingroup agn
     * 
     */
    class AGNSmoothTorusModel : public AGNStructureModel
    {
        PSimpleTorus m_torus;

        phys_float m_N_H;

    public:
        AGNSmoothTorusModel(const PSimpleTorus torus, phys_float N_H)
            : m_torus(torus), m_N_H{N_H}
        {
        }

        /**
         * @brief Get the distance to the next boundary inside the agn.
         * 
         * @param position reference point
         * @param direction direction of the trajectory
         * @return phys_float distance to the next boundary
         * @return -1   if there is no boundary for the given direction and reference point. 
         */
        virtual phys_float distanceToBoundary(const PPosition &position, const PVector3D &direction) const override
        {
            return m_torus.distanceToBoundaryFromInside(position, direction);
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
                                                   const PVector3D &direction)
        {
            return m_torus.distanceToBoundaryFromOutside(position, direction);
        }

        /**
         * @brief Get the equatorial half size of the agn structure model
         * 
         * @return phys_float 
         */
        virtual phys_float equatorialEffectiveHalfSize() const
        {
            return m_torus.externalRadius() - m_torus.internalRadius();
        }

        /**
         * @brief Returns true if the given point is inside the structure, 
         * otherwise false
         * 
         * @param position point position
         * @return phys_bool 
         */
        virtual phys_bool isInside(const PPosition &position) const
        {
            return m_torus.isInside(position);
        }

        /**
         * @brief Get the concentration of hydrogen
         * 
         * @return phys_float 
         */
        virtual phys_float n_H() const
        {
            return N_H / (m_torus.externalRadius() - m_torus.internalRadius());
        }
    };
} // namespace agn
