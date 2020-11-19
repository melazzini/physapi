#pragma once

#include "PVector3D.hpp"

namespace physapi
{
    /**
     * @addtogroup core
     * 
     * @{
     * 
     */

    /**
     * @brief This class represents a momentum vector.
     * 
     */
    class PMomentum : public PVector3D
    {

    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new zero PMomentum object
         * 
         */
        explicit PMomentum() : PVector3D{}
        {
        }

        /**
         * @brief Construct a new momentum with the specified coordinates.
         * 
         * 
         * @param x1 \f$ p_{x}\f(cartesian) or \f$ p_{\rho} \f$(polar) or p_{r}(spherical)
         * @param x2 \f$ p_{y}\f (cartesian) or \f$ \theta_{p} \f$(polar) or \f$ \theta_{p} \f$(spherical)
         * @param x3 \f$ p_{z}\f (cartesian) or \f$ p_{z} \f$(polar) or \f$ \phi_{p} \f$(spherical)
         * 
         * @param coordSys coordinate system
         * 
         * @note neither \f$ \rho \f$ nor r cannot be negative!
         */
        PMomentum(phys_float x1,
                  phys_float x2,
                  phys_float x3,
                  eCoordSys coordSys = eCoordSys::REC) : PVector3D{x1, x2, x3, coordSys}
        {
        }

        /**
         * @brief Construct a new momentum vector from a 3D vector.
         * 
         * @param vect3D 3D vector.
         */
        explicit PMomentum(const PVector3D &vect3D)
            : PVector3D(vect3D)
        {
        }

        ~PMomentum() {}
    };

    /**
     * @}
     * 
     */

} // namespace physapi
