#pragma once

#include "PPosition.hpp"
#include "PMomentum.hpp"

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
     * @brief This is the base class for all classical particles.
     * 
     * A classical particle in our api is defined by its position and momentum.
     * 
     */
    class PSimpleParticleBase
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
        PSimpleParticleBase(const PPosition &position = PPosition(),
                            const PMomentum momentum = PMomentum())
            : m_position{position}, m_momentum{momentum}
        {
        }

        /**
         * @brief Destroy the particle
         * 
         */
        virtual ~PSimpleParticleBase() {}

        /***********************************************************************
        *              GETTERS & SETTERS
        ***********************************************************************/

        /**
         * @brief Get the position of the particle
         * 
         * @return const PPosition 
         */
        const PPosition &position() const { return m_position; }

        /**
         * @brief Get the momentum of the particle
         * 
         * @return const PMomentum 
         */
        const PMomentum &momentum() const { return m_momentum; }

        /**
         * @brief Get the particle's energy.
         * 
         * @return phys_float 
         */
        virtual phys_float energy() const = 0;

        /**
         * @brief Get the particle's velocity
         * 
         * @return PVector3D 
         */
        virtual PVector3D velocity() const = 0;

        /**
         * @brief Set the position of the particle
         * 
         * @param position_ 
         */
        void setPosition(const PPosition &position_) { m_position = position_; }

        /**
         * @brief Set the momentum of the particle
         * 
         * @param momentum_ 
         */
        virtual void setMomentum(const PMomentum &momentum_) { m_momentum = momentum_; }

        /**
         * @brief Set the particle's energy
         * 
         * @param energy 
         */
        virtual void setEnergy(phys_float energy) = 0;

        /***********************************************************************
        *              PARTICLE MANIPULATION
        ***********************************************************************/

        /**
         * @brief Translate a particle a \f$ d\vec{r} \f$
         * 
         * @param dr 
         */
        virtual void translate(const PPosition &dr) { m_position += dr; }

        /**
         * @brief Translate a particle a length ds on the current direction
         * of movement.
         * 
         * If the momentum of the particle or velocity is zero, then this function
         * has no effect.
         * 
         * @param ds path length 
         */
        virtual void translate(phys_float ds)
        {
            auto momentumNorm = m_momentum.norm();
            if (momentumNorm > 0)
            {
                auto direction = m_momentum / momentumNorm;
                auto dr = ds * direction;
                m_position += dr;
            }
        }

        /**
         * @brief Give an impulse to the particle.
         * 
         * @param impulse 
         */
        virtual void giveImpulse(const PMomentum &impulse) = 0;

    protected:
        static constexpr void checkEnergy(phys_float energy)
        {
            if (energy < 0)
            {
                std::cerr << "PSimpleMassParticleBase::checkEnergy(energy): "
                          << std::endl;
                std::cerr << "The energy cannot be negative!" << std::endl;
                std::abort();
            }
        }

    private:
        // position of the particle
        PPosition m_position;

        // momentum of the particle
        PMomentum m_momentum;
    };

    /**
     * @}
     * 
     */

} // namespace physapi
