#pragma once

#include "PSimpleMassParticleBase.hpp"

namespace physapi
{
    /**
     * @addtogroup particles
     * 
     * @{
     * 
     */

    /**
     * @brief This class represents a simple particle.
     * 
     * A simple particle in our api has mass, velocity and position.
     * 
     * Both quantities can change.
     * 
     * The mass and the velocity are related by the definition of 
     * classical momentum   \f$ \vec{p} = m*\vec{v} \f$.
     * 
     * The energy is given by \f$ \frac{p^{2}}{2*m} \f$
     * and in case the mass is zero the energy will be also zero,
     * as well as the momentum, as well the velocity. When the momentum
     * of the particle is zero it does not interact with any other particle,
     * but it still can be moved to some position and it also can be given a mass.
     * 
     * @note If you have to work with particles which has a a fixed mass like an
     * electron for example, then derive from PSimpleMassParticleBase your own class.
     * 
     */
    class PSimpleParticle : public PSimpleMassParticleBase
    {
    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new simple particle
         * 
         * @param position initial position
         * @param momentum initial momentum
         */
        explicit PSimpleParticle(const PPosition &position = PPosition(),
                                 const PMomentum &momentum = PMomentum())
            : PSimpleMassParticleBase(position, momentum)
        {
        }

        /**
         * @brief Construct a new simple particle with mass, position and velocity
         * 
         * @param mass initial mass
         * @param position initial position
         * @param velocity initial velocity
         */
        PSimpleParticle(phys_float mass,
                        const PPosition &position,
                        const PVector3D &velocity = PVector3D())
            : PSimpleMassParticleBase(position, PMomentum(mass * velocity)),
              m_mass{mass}
        {
            checkMass(mass);
        }

        /**
         * @brief Destroy the particle
         * 
         */
        virtual ~PSimpleParticle() {}

        /***********************************************************************
        *              GETTERS & SETTERS
        ***********************************************************************/

        /**
         * @brief Get the mass of the particle
         * 
         * @return phys_float 
         */
        phys_float mass() const override { return m_mass; }

        /**
         * @brief Get the particle's energy.
         * 
         * @return phys_float 
         */
        virtual phys_float energy() const override
        {
            return (m_mass > 0) ? sqr(momentum()) / (2 * m_mass) : 0.0;
        }

        /**
         * @brief Get the particle's velocity
         * 
         * @return PVector3D 
         */
        virtual PVector3D velocity() const override
        {
            return (m_mass > 0) ? momentum() / m_mass : PVector3D();
        }

        /**
         * @brief Set the particle's energy
         * 
         * This will work only if the particle's mass is not zero.
         * 
         * It will set the energy only in case the momentum
         * of the particle is not zero, because you need a
         * direction.
         * 
         * @param energy 
         */
        virtual void setEnergy(phys_float energy) override
        {
            checkEnergy(energy);

            if (m_mass > 0 && momentum().r() != 0)
            {
                // get a unitary vector in the direction of
                // the current momentum
                PVector3D momentum_ = momentum();

                momentum_.setR(sqrt(2 * m_mass * energy));

                setMomentum(PMomentum{momentum_});
            }
        }

        /**
         * @brief Set the particle's mass
         * 
         * @param mass 
         */
        virtual void setMass(phys_float mass)
        {
            checkMass(mass);

            m_mass = mass;
            setMomentum(PMomentum(m_mass * velocity()));
        }

        /**
         * @brief Set the particle energy with a given direction
         * 
         * @param energy energy
         * @param direction direction
         */
        virtual void setEnergy(phys_float energy, PVector3D &&direction)
        {
            if (m_mass > 0 && direction.r() != 0)
            {
                direction.setR(sqrt(2 * m_mass * energy));

                setMomentum(PMomentum(direction));
            }
        }

        /**
         * @brief Set the Velocity object
         * 
         * This will work only if the particle's mass is not zero.
         * 
         * @param velocity 
         */
        virtual void setVelocity(const PVector3D &velocity)
        {
            if (m_mass > 0)
            {
                setMomentum(PMomentum(m_mass * velocity));
            }
        }

        /**
         * @brief Give an impulse to the particle.
         * 
         * This will work only if the particle's mass is not zero.
         * 
         * @param impulse 
         */
        virtual void giveImpulse(const PMomentum &impulse)
        {
            if (m_mass > 0)
            {
                setMomentum(PMomentum(momentum() + impulse));
            }
        }

    private:
        // particle's mass
        phys_float m_mass;

    private:
        static constexpr void checkMass(phys_float mass)
        {
            if (mass < 0)
            {
                std::cerr << "PSimpleParticle::checkMass(mass): "
                          << std::endl;
                std::cerr << "The mass cannot be negative!" << std::endl;
                std::abort();
            }
        }
    };

    /**
     * @}
     * 
     */
} // namespace physapi
