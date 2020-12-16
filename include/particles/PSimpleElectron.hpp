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
     * @brief This class represents a simple electron.
     * 
     * A simple electron in our api is defined by its position and  momentum.
     * Its mass is constant and equal to physapi::m_e
     * 
     */
    class PSimpleElectron : public PSimpleMassParticleBase
    {
    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new simple electron with zero momentum at
         * the origin.
         * 
         */
        PSimpleElectron()
            : PSimpleMassParticleBase()
        {
        }

        /**
         * @brief Construct a new electron
         * 
         * @param position initial position
         * @param momentum initial momentum
         */
        PSimpleElectron(const PPosition &position, const PMomentum &momentum = PMomentum())
            : PSimpleMassParticleBase(position, momentum)
        {
        }

        /**
         * @brief Construct a new electron
         * 
         * @param position initial position
         * @param velocity initial velocity
         */
        PSimpleElectron(const PPosition &position, const PVector3D &velocity)
            : PSimpleMassParticleBase(position, PMomentum(m_e * velocity))
        {
        }

        /**
         * @brief Destroy the simple electron
         * 
         */
        virtual ~PSimpleElectron() {}

        /***********************************************************************
        *              GETTERS & SETTERS
        ***********************************************************************/

        phys_float mass() const override { return m_e; }

        /**
         * @brief Get the electron's energy.
         * 
         * @return phys_float 
         */
        virtual phys_float energy() const
        {
            return momentum().normSquared() / (2 * m_e);
        }

        /**
         * @brief Get the electron's velocity
         * 
         * @return PVector3D 
         */
        virtual PVector3D velocity() const
        {
            return momentum() / m_e;
        }

        /**
         * @brief Set the electron's energy
         * 
         * @param energy 
         */
        virtual void setEnergy(phys_float energy)
        {
            if (momentum().r() != 0)
            {
                PVector3D momentum_{momentum()};

                momentum_.setR(sqrt(2 * m_e * energy));

                setMomentum(PMomentum(momentum_));
            }
        }

        /**
         * @brief Set the electron's velocity
         * 
         * @param velocity_
         */
        virtual void setVelocity(const PVector3D &velocity_)
        {
            setMomentum(PMomentum(m_e * velocity_));
        }

        /**
         * @brief Give an impulse to the electron.
         * 
         * @param impulse 
         */
        virtual void giveImpulse(const PMomentum &impulse)
        {
            setMomentum(PMomentum(momentum() + impulse));
        }

        /**
         * @brief Get the direction of movement
         * 
         * @return PVector3D 
         * 
         * @note it returns zero if the momentum is zero
         */
        PVector3D omega() const
        {
            if (momentum().norm() == 0)
            {
                return PVector3D();
            }

            return momentum() / momentum().norm();
        }
    };

    /**
     * @}
     * 
     */

} // namespace physapi
