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
     * @brief This class represents a simple photon.
     * 
     * A simple photon in our api is defined by its position,
     * energy and direction of movement.
     * 
     */
    class PSimplePhoton : public PSimpleParticleBase
    {
    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new photon
         * 
         * @param energy initial energy
         * @param direction initial direction
         * @param position initial position
         * 
         * @note a photon with initial energy 0 cannot interact with other particles
         */
        PSimplePhoton(phys_float energy,
                      const PVector3D &direction,
                      const PPosition &position = PPosition())
            : PSimpleParticleBase(position,
                                  PMomentum((energy / c_light) * (direction / direction.norm()))),
              m_energy{energy}
        {
            checkEnergy(energy);
        }

        /**
         * @brief Destroy the photon
         * 
         */
        virtual ~PSimplePhoton() {}

        /***********************************************************************
        *              GETTERS & SETTERS
        ***********************************************************************/

        /**
         * @brief Get the photon's energy.
         * 
         * @return phys_float 
         */
        phys_float energy() const override { return m_energy; }

        /**
         * @brief Get the photon's velocity
         * 
         * @return PVector3D 
         * 
         * @note it returns zero if the momentum is zero
         * otherwise it returns the speed of light with
         * the corresponding direction of movement
         * 
         */
        virtual PVector3D velocity() const
        {
            return c_light * omega();
        }

        /**
         * @brief Set the photon's energy
         * 
         * @param energy 
         */
        virtual void setEnergy(phys_float energy)
        {
            checkEnergy(energy);
            setMomentum(PMomentum((energy / c_light) * omega()));
        }

        /**
         * @brief Photon's direction of movement
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

        /**
         * @brief Set the photon's direction of movement
         * 
         * @param direction
         * 
         * @note this works only if the photon's momentum
         * is not zero
         */
        void setOmega(const PVector3D &direction)
        {
            auto norm{momentum().norm()};
            if (norm == 0)
            {
                return;
            }

            setMomentum(PMomentum(norm * (direction / direction.norm())));
        }

        void setMomentum(const PMomentum &momentum_) override
        {
            m_energy = momentum_.norm() * c_light;

            PSimpleParticleBase::setMomentum(momentum_);
        }

        /***********************************************************************
        *              PARTICLE MANIPULATION
        ***********************************************************************/
        /**
         * @brief Give an impulse to the photon.
         * 
         * @param impulse 
         */
        virtual void giveImpulse(const PMomentum &impulse)
        {
            setMomentum(PMomentum(momentum() + impulse));
        }

    private:
        // photon energy
        phys_float m_energy;
    };

    /**
     * @}
     * 
     */
} // namespace physapi
