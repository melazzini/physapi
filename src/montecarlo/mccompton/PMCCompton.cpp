#include "PMCCompton.hpp"
#include "_PMCComptonElectronModeler.hpp"

namespace physapi
{
    void PMCCompton::run(phys_float T_e)
    {
        PSimpleElectron electron;

        _PMCComptonElectronModeler electronModeler{T_e};

        //first we need to get the values of the parameters
        //x and \gamma
        auto &&[x_, gamma_, beta_]{electronModeler(m_photon, electron)};

        //set the initial energy of the photon, which is
        //needed to get x'(see below)
        m_hv0 = m_photon.energy();

        //we need to get the direction of the electron and
        //photon initially:
        m_Omega = m_photon.omega();

        m_v0 = electron.omega();

        //we set xi_3 and m_Y first so that
        //we will enter the loop at least once:
        xi_3 = 1;
        m_Y = 0;

        while (2 * xi_3 >= m_Y)
        {
            //we need to set xi_1 and xi_2
            //to a radom value on [0,1] so
            //that we can get the next
            //variables
            xi_1 = m_randomMng.random_0_1();
            xi_2 = m_randomMng.random_0_1();

            //we need to se \mu' and \phi' in order to get
            //the values for \Omega' coordinates, according
            //to the theory

            m_mu_prime = (beta_ + 2 * xi_1 - 1) /
                         (1 + beta_ * (2 * xi_1 - 1));

            m_phi_prime = 2 * Pi * xi_2;

            //now we can set the coordinates of the new direction vector \Omega',
            //this vector is also needed to get the value of the parameter x'

            setOmegaPrime();

            //now we can set the value of the parameter x', needed to get
            //the value of m_Y, which is need to select when to accept
            //the values of the new direction \Omega', in our implementation
            //of the algorithm we need to pass the values of x and gamma,
            //calculated before when we model the electron momentum

            m_x_prime = getXPrime(x_, gamma_, beta_);

            //and here we already can get the value of Y
            m_Y = getYPrime(x_);

            //finally we set a random number to select whether or not
            //accept the values and end the while loop
            xi_3 = m_randomMng.random_0_1();
        }

        //after finising the previous while loop
        //we are able now to set the new value of
        //the energy of the photon and its new
        //direction

        auto &&energy = getNewPhotonEnergy(gamma_, beta_);

        m_photonMomentum = PMomentum((energy/c_light)*m_OmegaPrime);
    }
} // namespace physapi
