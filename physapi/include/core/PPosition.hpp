#pragma once

#include "PVector3D.hpp"

namespace physapi
{
    /**
     * @addtogroup  core
     * 
     * @{
     * 
     */

    /**
     * @brief This class represents a radius vector.
     * 
     */
    class PPosition : public PVector3D
    {
    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new radius vector with x=y=z=0.
         * 
         */
        explicit PPosition()
            : PVector3D{}
        {
        }

        /**
         * @brief Construct a new radius vector with the specified coordinates.
         * 
         * As you can see, if you need to create a 3D-vector in rectangular
         * coordinates, then x1 represents x and x2 represents y, x3 represents z. 
         * On the other hand, if you need a polar vector, then x1 represents \f$ \rho \f$
         * x2 represents \f$ \theta \f$, and x3 represents \f$ \phi \f$.
         * 
         * @param x1 x(cartesian) or \f$ \rho \f$(polar) or r(spherical)
         * @param x2 y(cartesian) or \f$ \theta \f$(polar) or \f$ \theta \f$(spherical)
         * @param x3 z(cartesian) or \f$ z \f$(polar) or \f$ \phi \f$(spherical)
         * 
         * @param coordSys coordinate system
         * 
         * @note neither \f$ \rho \f$ nor r cannot be negative!
         */
        PPosition(phys_float x1,
                  phys_float x2,
                  phys_float x3,
                  eCoordSys coordSys = eCoordSys::REC) : PVector3D{x1, x2, x3, coordSys}
        {
        }

        /**
         * @brief Construct a new radius vector from a 3D vector.
         * 
         * @param vect3D 3D vector.
         */
        explicit PPosition(const PVector3D &vect3D)
            : PVector3D(vect3D)
        {
        }

        /**
         * @brief Destroy the PPosition object
         * 
         */
        virtual ~PPosition() {}

        /***********************************************************************
        *              OTHER UTILITIES
        ***********************************************************************/

        /**
         * @brief Calculate the distance from this position to the other.
         * 
         * @param other other position
         * @return phys_float distance
         */
        phys_float distanceToPoint(const PPosition &other) const
        {
            return (other - *(this)).norm();
        }
    };

    /**
     * @}
     * 
     */
} // namespace physapi
