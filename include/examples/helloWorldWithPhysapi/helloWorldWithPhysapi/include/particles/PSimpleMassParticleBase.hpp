#pragma once

#include "PSimpleParticleBase.hpp"
/**
 * @defgroup    particles
 * 
 * @brief This module handles particles definitions.
 * 
 * @{
 * @}
 * 
 */

namespace physapi
{
    /**
     * @addtogroup particles
     * 
     * @{
     * 
     */

    /**
     * @brief This is the base class for all classical particles that have an inertial mass.
     * 
     * A classical particle in our api is defined by its position and momentum.
     * 
     */
    class PSimpleMassParticleBase : public PSimpleParticleBase
    {
    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new particle
         * 
         * @param position initial position of the particle
         * @param momentum initial momentum of the particle
         */
        PSimpleMassParticleBase(const PPosition &position = PPosition(),
                                const PMomentum momentum = PMomentum())
            : PSimpleParticleBase(position, momentum)
        {
        }

        /**
         * @brief Destroy the particle
         * 
         */
        virtual ~PSimpleMassParticleBase() {}

        /***********************************************************************
        *              GETTERS & SETTERS
        ***********************************************************************/

        /**
         * @brief Get the mass of the particle
         * 
         * @return phys_float 
         */
        virtual phys_float mass() const = 0;

        /**
         * @brief Set the mass of the particle
         * 
         * @param mass 
         */
        virtual void setMass(phys_float mass) {}
    };

    /**
     * @}
     * 
     */

} // namespace physapi
