#pragma once

#include "utils.hpp"
#include "PSimpleMassParticleBase.hpp"

namespace physapi
{

    /**
     * @brief This is the base class for all types of classical collisions.
     * 
     * @ingroup collision
     * 
     */
    class PCollisionBase
    {
    protected:
        /**
         * @brief Construct a new PCollisionBase object
         * 
         */
        PCollisionBase() {}

    public:
        /**
         * @brief Destroy the PCollisionBase object
         * 
         */
        virtual ~PCollisionBase() {}

        /**
         * @brief Get the momentum of the particle A
         * 
         * @return PMomentum 
         */
        virtual const PMomentum &momentumA() const = 0;

        /**
         * @brief Get the momentum of the particle B
         * 
         * @return PMomentum 
         */
        virtual const PMomentum &momentumB() const = 0;
    };
} // namespace physapi
