#pragma once

#include "PCollisionBase.hpp"

namespace physapi
{

    /**
     * @brief Handles perfectly elastic collisions between two particles.
     * 
     * @ingroup collision
     * 
     */
    class PPerfectlyElasticCollision : public PCollisionBase
    {
    private:
        const PSimpleMassParticleBase &m_particleA; // reference to the particle A

        const PSimpleMassParticleBase &m_particleB; // reference to the particle B

        PMomentum m_momentumA_prime; // resulting momentum of the particle A
        PMomentum m_momentumB_prime; // resulting momentum of the particle B

    public:
        /**
         * @brief Construct a new PPerfectlyElasticCollision object
         * 
         * @param partileA particle A
         * @param partileB particle B
         */
        PPerfectlyElasticCollision(const PSimpleMassParticleBase &partileA,
                                   const PSimpleMassParticleBase &partileB)
            : m_particleA{partileA}, m_particleB{partileB}
        {
            run();
        }

        /**
         * @brief Destroy the PPerfectlyElasticCollision object
         * 
         */
        virtual ~PPerfectlyElasticCollision() {}

        /**
         * @brief Get the momentum of the particle A
         * 
         * @return const PMomentum& 
         */
        virtual const PMomentum &momentumA() const override { return m_momentumA_prime; }

        /**
         * @brief Get the momentum of the particle A
         * 
         * @return const PMomentum& 
         */
        virtual const PMomentum &momentumB() const override { return m_momentumB_prime; }

    private:
        // run the collision
        void run();
    };
} // namespace physapi
