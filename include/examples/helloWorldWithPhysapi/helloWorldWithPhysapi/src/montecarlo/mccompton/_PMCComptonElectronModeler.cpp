#include "_PMCComptonElectronModeler.hpp"

namespace physapi
{
    std::tuple<phys_float, phys_float, phys_float>
    _PMCComptonElectronModeler::operator()(const PSimplePhoton &photon, PSimpleElectron &electron)
    {
        //what we are looking for in this method and
        //precisely in the while loop below is
        //the momentum of the electron

        //first we get the direction of the photon
        m_Omega = photon.omega();

        //we get the photon energy
        hv = photon.energy();

        //now we initialize \mu, which
        //is the dot product of two
        //vectors: \Omega and v^0
        // m_mu = m_Omega * m_v0;
        m_mu = 0;

        //now we need to reset the values of the \xi
        //and \sigma_compton parameters to 1 and 0,
        //so that we can perform at least one
        //simulation inside the while loop
        m_xi = 1;
        m_sigma_compton = 0;

        //we have now two cases: cold and hot electrons

        //according to the algorithm we need to test the next
        //condition
        //p 138
        while (m_xi >= (0.375 * m_sigma_compton * (1 - m_mu * m_beta)))
        {
            //in this while loop we are looking for
            //the value of the parameter \eta, which
            //represents the momentum of the electron

            //we will select the algorithm to get
            //the parameter \eta according to the
            //temperature of the electrons
            //p: 130

            //for cold electrons:
            if (k_B * m_T_e <= 150E3_eV)
            {
                m_eta = getEta_ColdElectrons();
            }
            //for hot electrons:
            else
            {
                m_eta = getEta_HotElectrons();
            }

            //once we have \eta we can get \gamma and \beta:
            m_gamma = std::sqrt(sqr(m_eta) + 1);
            m_beta = m_eta / m_gamma;

            //now we need a random direction of
            //the electron momentum
            m_RandomDirectionHnd(&m_v0);

            //now we calculate \mu, which
            //is the dot product of two
            //vectors: \Omega and v^0
            m_mu = m_Omega * m_v0;

            //now we can get the value for the parameter x
            //p 138
            m_x = 2 * m_gamma * (hv / mc2_e) * (1 - m_mu * m_beta);

            if (m_x < 0.5)
            {
                m_sigma_compton = getSigmaFirstCase();
            }
            else if (m_x >= 0.5 && m_x <= 3.5)
            {
                m_sigma_compton = getSigmaSecondCase();
            }
            else
            {
                m_sigma_compton = getSigmaThirdCase();
            }

            //we choose a random value for xi
            //on [0,1)
            m_xi = m_randomMngr.random_0_1();
        }

        //now having \eta we can set the momentum of
        //the electron, according to the theory:
        electron.setMomentum(

            PMomentum((m_beta * mc_e) * m_v0)
            // (v/c)*m*c = mv
        );

        //the value of x and \gamma must be returned so that
        //it can be possible to simulate the compton scattering
        return std::make_tuple(m_x, m_gamma, m_beta);
    }

    phys_float _PMCComptonElectronModeler::getEta_ColdElectrons()
    {

        xi_1 = 0;
        // m_randomMngr.random_0_1();
        xi_2 = 0;
        // m_randomMngr.random_0_1();

        zeta_prime = 0;
        // -(3.0 / 2.0) * std::log(xi_1);

        while (sqr(xi_2) >=
               (0.151 *
                sqr(1 + n * zeta_prime) *
                zeta_prime * (2 + n * zeta_prime) *
                xi_1))
        {
            xi_1 = m_randomMngr.random_0_1();
            xi_2 = m_randomMngr.random_0_1();
            zeta_prime = -(3.0 / 2.0) * std::log(xi_1);
        }

        return std::sqrt(n * zeta_prime * (2 + n * zeta_prime));
    }

    phys_float _PMCComptonElectronModeler::getEta_HotElectrons()
    {

        eta_prime = 0;
        eta_double_prime = 0;

        while (sqr(eta_double_prime) - sqr(eta_prime) <= 1)
        {
            xi_1 = m_randomMngr.random_0_1();
            xi_2 = m_randomMngr.random_0_1();
            xi_3 = m_randomMngr.random_0_1();
            xi_4 = m_randomMngr.random_0_1();

            eta_prime = -n * std::log(xi_1 * xi_2 * xi_3);
            eta_double_prime = -n * std::log(xi_1 * xi_2 * xi_3 * xi_4);
        }

        return eta_prime;
    }

} // namespace physapi
