#pragma once
#include "PRandomDirection.hpp"
#include "PSimplePhoton.hpp"
#include "PSimpleElectron.hpp"
#include <tuple>

namespace physapi
{
    //this class models the momentum of a simple electron(PSimpleElectron)
    //using motecarlo methods in order to simulate compton scattering,
    //we use this class as a functor, and pass to the ctor all the required
    //parameters, notice that according to the theory to simulate "after" the
    //compton scattering we also need a parameter "x", which is obtained
    //when we model the momentum, ie with the present class we also will get
    //the value of that parameter
    class _PMCComptonElectronModeler
    {
    public:
        //the only required "configuration" parameter in order to model
        //the momentum of the electrons is its temperature
        _PMCComptonElectronModeler(const phys_float T_e)
            //in this ctor we create \Omega and v_0 unit vectors
            : m_T_e{T_e}, m_Omega{1, 0, 0}, m_v0{1, 0, 0}
        {
        }

        ~_PMCComptonElectronModeler() {}

        //it receives a photon and electron references and returns the value
        //of the parameter x and \gamma and beta, which are needed for simulating the
        //scattering it also sets the momentum of the electron according
        //to montecarlo techniques, notice that we return a tuple:
        //where the first element is the value of x and the second \gamma
        //and the third beta
        std::tuple<phys_float, phys_float, phys_float>
        operator()(const PSimplePhoton &photon, PSimpleElectron &electron);

    private:
        //temperature of the electrons*
        const phys_float m_T_e; //with it we get the parameter n:
                                // n = kT_e/mc^2
                                //p: 130

        //momentum of the electron: \eta=p/(mc), where
        //p-relativistic momentum of the electron*
        phys_float m_eta; //p: 129:

        //parametro: \gamma=(\eta^2 + 1)^{1/2}*
        phys_float m_gamma; //p: 138

        //this variable is for selecting the value of the momentum
        //according to montecarlo methods, m_xi will hold
        //random numbers in [0,1]
        phys_float m_xi; //p: 138

        //compton cross-section*
        phys_float m_sigma_compton; //p: 131

        //\beta = v/c = \eta/\gama*
        phys_float m_beta{}; //p: 138

        //cosine between the direction(unit vector)
        //of the photon-\Omega and the direction
        //(unit vector) of the electron v^0:
        // \mu = v^0 \Omega*
        phys_float m_mu; //p: 138

        //parameter to get \sigma_{compton} and
        //to be able to simulate the compton
        //scattering after having the momentum
        //of the electron, we return the value
        //of this parameter with the functor*
        phys_float m_x; //p: 131, 136

        //unit vector photon direction
        PVector3D m_Omega;

        //unit vector electron direction
        PVector3D m_v0;

        //this member sets a random dir to
        //the momentum of the electron
        PRandomDirection m_RandomDirectionHnd;

    private:
        //here we have some variables that help us
        //store random values and use them in
        //internal helper functions, which update
        //every time they are called the values of
        //the needed variables, the names of these
        //variables have been taken according to
        //those used in the book, for example see
        //page 130
        phys_float xi_1, xi_2, xi_3, xi_4;

        //\zeta' = -(3\2)ln(xi_1), where xi_1 is
        //a random value on [0,1)
        //p: 130
        phys_float zeta_prime;

        //\eta' = nln(\xi_1 \xi_2 \xi_3)
        //p: 130
        phys_float eta_prime;

        //\eta'' = nln(\xi_1 \xi_2 \xi_3 \xi_4)
        //p: 130
        phys_float eta_double_prime;

        //electron energy paramater: n = k_{B}T_e / mc^2
        const phys_float n = k_B * m_T_e / mc2_e;

        //energy of the current photon
        phys_float hv;

        //this object manages the internal production of random
        //numbers necessary to run the montecarlo simulation
        PRandom m_randomMngr;

    private:
        //helper function to get the value of
        //the momentum parameter \eta when
        //the electrons are cold
        //p: 130
        phys_float getEta_ColdElectrons();

        //helper function to get the value of
        //the momentum parameter \eta when
        //the electrons are hot
        //p: 130
        phys_float getEta_HotElectrons();

        //helper internal function: returns the value
        //of the compton scattering crosssection
        //use it when m_x < 0.5
        //p: 131
        phys_float getSigmaFirstCase()
        {
            return (1.0 / 3.0) +
                   0.141 * m_x -
                   0.12 * m_x * m_x +
                   (1 + 0.5 * m_x) / sqr(1 + m_x);
        }

        //helper internal function: returns the value
        //of the compton scattering crosssection,
        //use it when m_x >= 0.5 && m_x <= 3.5
        //p: 131
        phys_float getSigmaSecondCase()
        {
            return (std::log(1.0 + m_x) + 0.06) / m_x;
        }

        //helper internal function: returns the value
        //of the compton scattering crosssection,
        //use it when m_x>3.5
        //p: 131
        phys_float getSigmaThirdCase()
        {
            return (log(1.0 + m_x) + 0.5 - 1.0 / (2.0 + 0.076 * m_x)) /
                   m_x;
        }
    };
} // namespace physapi
