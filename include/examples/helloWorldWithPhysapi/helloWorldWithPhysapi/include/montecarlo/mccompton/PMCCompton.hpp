#pragma once

#include "PSimpleElectron.hpp"
#include "PSimplePhoton.hpp"

/**
 * @defgroup mccompton MC Compton
 * 
 * @brief This submodule contains all the tools needed for simulation of compton 
 * scattering with Monte Carlo methods.
 * 
 * @ingroup montecarlo
 * 
 */

namespace physapi
{
    /**
     * @brief Handles compton interactions with Monte Carlo methods.
     * 
     * @ingroup mccompton
     * 
     */
    class PMCCompton
    {
    private:
        // momentum of the photon
        PMomentum m_photonMomentum;

        // photon
        const PSimplePhoton &m_photon;

    public:
        /**
         * @brief Construct a new PMCCompton object
         * 
         */
        PMCCompton(const PSimplePhoton &photon, phys_float T_e)
            : m_photon{photon}
        {
            run(T_e);
        }

        /**
         * @brief Destroy the PMCCompton object
         * 
         */
        virtual ~PMCCompton() {}

        /**
         * @brief Get the photon momentum
         * 
         * @return const PMomentum& 
         */
        const PMomentum &photonMomentum() const { return m_photonMomentum; }

    private:
        // it receives the temperature of the electron
        void run(phys_float T_e);

        /*********************************************************************
         *      data members needed for the montecarlo simulation
         *********************************************************************/

    private:
        //we need x to answer: when does the
        //compton scattering occur? in other
        //words what will be the new direction
        //and energy of the photon, according
        //to montecarlo methods
        phys_float m_x;

        //initial energy of the photon,
        //ie the energy of the photon
        //before the interaction with
        //the electron
        phys_float m_hv0;

        //parameter to get the frequency shift:
        //\mu' = \frac{v/c + 2\xi_1 - 1}{1+ (v/c)(2\xi_1 - 1)}*
        phys_float m_mu_prime; //p: 136

        //parameter to get the frequency shift:
        //x' = x \bigg[ 1 +
        //\frac{h\nu (1- \bold{\Omega} \bold{\Omega'})}
        //     {\gamma mc^2(1 - \mu' v/c)} \bigg]^{-1}
        phys_float m_x_prime; //p: 136

        //factor Y=(x'/x)^2 X*
        phys_float m_Y; //p: 136

        //parameter to get the change in direction
        //of the photon:
        //\phi' = 2\pi \xi_2*
        phys_float m_phi_prime; //p: 136

        //initial direction of the photon,
        //it is a unit vector
        PVector3D m_Omega;

        //new direction of the photon,
        //unit vector
        PVector3D m_OmegaPrime;

        //initial direction of the electron,
        //it is a unit vector
        PVector3D m_v0;

    private:
        //here we have some variables that help us
        //store random values and use them internally
        phys_float xi_1, xi_2, xi_3, xi_4;

        PVector3D w, t;

        //this object manages the internal production of random
        //numbers necessary to run the montecarlo simulation
        PRandom m_randomMng;

    private:
        //this helper internal function is used for
        //setting the value of the parameter x', which
        //is needed to get the value of the Y parameter
        phys_float getXPrime(const phys_float &x, const phys_float &gamma, const phys_float &beta);

        //this helper internal function is used for
        //setting the value of the parameter Y, which
        //is need to determine when the values of
        //\Omega', x' and \mu' are accepted
        phys_float getYPrime(const phys_float &x);

        //this is an internal helper function,
        //it receives the parameter \gamma and
        //it returns the new energy of the photon
        phys_float getNewPhotonEnergy(const phys_float &gamma, const phys_float &beta);

        //this helper internal function is used for
        //setting the value of the new direction-unit
        //vector of the photon Omega'
        //p: 135
        void setOmegaPrime();
    };

    inline phys_float PMCCompton::getXPrime(const phys_float &x, const phys_float &gamma, const phys_float &beta)
    {
        return x / (1.0 + (m_hv0 * (1.0 - (m_Omega * m_OmegaPrime))) /
                              (gamma * mc2_e * (1.0 - m_mu_prime * beta)));
    }

    inline phys_float PMCCompton::getYPrime(const phys_float &x)
    {
        return sqr(m_x_prime / x) *
               (x / m_x_prime +
                m_x_prime / x + 4.0 * (1.0 / x - 1.0 / m_x_prime) +
                4.0 * sqr(1.0 / x - 1.0 / m_x_prime));
    }

    inline phys_float
    PMCCompton::getNewPhotonEnergy(const phys_float &gamma, const phys_float &beta)
    {
        return m_x_prime * mc2_e / (2 * gamma * (1 - m_mu_prime * beta));
    }

    inline void PMCCompton::setOmegaPrime()
    {
        phys_float &&ro = std::sqrt(sqr(m_v0[0]) + sqr(m_v0[1]));

        w.setX(m_v0[1] / ro);

        w.setY(-m_v0[0] / ro);

        w.setZ(0);

        t.setX(m_v0[0] * m_v0[2]);

        t.setY(m_v0[1] * m_v0[2]);

        t.setZ(-sqr(ro));

        t /= ro;

        m_OmegaPrime = m_mu_prime * m_v0 +
                       std::sqrt(1 - sqr(m_mu_prime)) *
                           (w * std::cos(m_phi_prime) +
                            t * std::sin(m_phi_prime));
    }

} // namespace physapi
