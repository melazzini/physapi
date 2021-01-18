#pragma once

#include "PSpectrum.hpp"
#include "PFluxPerEnergyInterval.hpp"

namespace physapi
{
    /**
     * @brief This class represents the flux per energy interval
     * 
     * @tparam energyIntervals number of energy energy intervals
     * 
     * Flux in physics is energy per area(and per seconds).
     * 
     * Therefore, an instance of PFluxPerEnergyInterval represents:
     * 
     * \f$ \Phi_{\Delta E} = \frac{E}{\Delta A * \Delta E * s} \f$     
     * 
     * 
     * As we can see the units are \f$ Joules \f$ per \f$ m^{2}*sec*Joules \f$
     * 
     * In our api we can rewrite that formula in the following form:
     * 
     * \f$ \Phi_{\Delta E, normalized}(E_{i}) = \alpha * \frac{E_{i} * \Delta N(E_{i})}{\Delta A * \Delta E(E_{i}) * s} \f$
     * 
     * Where \f$ E_{i} \f$ is the energy of the correspoding interval, \f$ \Delta N(E_{i}) \f$ is the number of photons
     * in the correspoding energy interval \f$ \Delta E(E_{i}) \f$
     * and \f$ \alpha \f$ is a normalization parameter.
     * 
     * Let's suppose that you need to normalize the given flux per energy interval, so that when the energy is \f$ E_{i}=E^{*} \f$
     * the \f$ \Phi_{\Delta E, normalized} \f$ is \f$ \eta \f$. Therefore, we will have that:
     * 
     * \f$  \alpha = \eta(E^{*}) * \frac{\Delta A \Delta E(E^{*})}{E^{*} \Delta N(\Delta{E^{*}}) } \f$
     * 
     * We can choose a normalization flux per energy interval, such that we know \f$ \Delta A_{0} \f$, \f$ \Delta E(E^{*}) \f$
     * and \f$ \Delta N_{0}(E^{*}) \f$ for that \f$ \Phi_{0, \Delta E, normalized} \f$. If we do so, any other given flux
     * per energy interval will be given by:
     * 
     * \f[ \Phi_{\Delta E, normalized}(E_{i}) = \eta * \frac{\Delta A_{0}}{\Delta A} * \frac{E_{i}}{E^{*}} * \frac{\Delta N(E_{i})}{\Delta N_{0}(E^{*})} * \frac{\Delta E(E^{*})}{\Delta E(E_{i})} \f]
     * 
     * Therefore the PFluxPerEnergyIntervalMaker receives in the constructor:
     * 
     *  * \f$ \eta \f$
     * 
     *  * \f$ \Delta A_{0} \f$
     * 
     *  * \f$ E^{*} \f$
     * 
     *  * the normalization spectrum(instance of PSpectrum)
     * 
     * The normalization spectrum helps us to determine \f$ \Delta N_{0}(E^{*}) \f$. It can be, for example the intrinsic spectrum
     * of the source.
     * 
     * **Example:**
     * 
     * In the next example we create an intrinsic active galactic nuclei(AGN) spectrum, from which
     * we build the correspoding flux per energy interval. 
     * 
     @code{.cpp}

        #include "PSpectrum.hpp"
        #include "PSpectrumMaker.hpp"
        #include "PFluxPerEnergyIntervalMaker.hpp"
        #include "PFluxPerEnergyInterval.hpp"
        #include <cmath>
        #include <fstream>

        using namespace std;
        using namespace physapi;

        class AGNFormula : public PSpectralFormulaBase
        {
        public:
            constexpr static phys_float E_cut = 300.0E3;
            constexpr static phys_float E_low = 1.0;
            constexpr static phys_float gamma = 1.8;
            constexpr static phys_size N_intervals = 5000;

            phys_float operator()(phys_float energy) const override
            {
                return std::pow(energy, -gamma) * std::exp(-energy / E_cut);
            }
        };

        int main()
        {
            const phys_float numberOfPhotons = 1.5E9;

            PSpectrum<AGNFormula::N_intervals> intrinsicSpectrum{
                AGNFormula::E_low,
                AGNFormula::E_cut,
                eSpectrumScale::LOG};

            AGNFormula myFormula;

            PSpectrumMaker<AGNFormula::N_intervals, 100'000'000> spectrumMaker;

            spectrumMaker(intrinsicSpectrum, numberOfPhotons, myFormula);

            PFluxPerEnergyIntervalMaker<AGNFormula::N_intervals> fluxMaker{1.0E3, 4 * Pi, 1.0E3, intrinsicSpectrum};

            PFluxPerEnergyInterval<AGNFormula::N_intervals> flux_dE(fluxMaker(intrinsicSpectrum, 4 * Pi));

            ofstream fout;
            fout.open("myFile");

            fout << flux_dE << endl;

            return 0;
        }

        @endcode
     * 
     * output:
     * 
     * <a href="myFile">myFile</a>
     *
     * @image html AGN_intrinsicFluxExample.png
     * 
     * @ingroup radiation
     * 
     * @warning All spectra processed by this class, if they are empty they need to be equal to each other and to the normalization spectrum,
     * when it is empty.
     * 
     */
    template <phys_size energyIntervals>
    class PFluxPerEnergyIntervalMaker
    {
    private:
        std::string m_unitsX;

        std::string m_unitsY;

    private:
        // normalization value
        phys_float m_eta;

        // area for the normalization flux per energy interval
        phys_float m_dA0;

        // normalization energy
        phys_float m_E;

        // number of photons at the normalization energy interval
        // for the normalization spectrum
        phys_float m_dN0;

        // delta energy of the correspoding interval of the normalization
        // energy m_E
        phys_float m_dE0;

    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new PFluxPerEnergyIntervalMaker object
         * 
         * @param eta 
         * @param dA0 
         * @param E 
         * @param normalizationSpectrum 
         */
        PFluxPerEnergyIntervalMaker(phys_float eta,
                                    phys_float dA0,
                                    phys_float E,
                                    const PSpectrum<energyIntervals> &normalizationSpectrum,
                                    const std::string &unitsX = "j", const std::string &unitsY = "j,j^{-1},m^{-2},s^{-1}")
            : m_eta{eta}, m_dA0{dA0}, m_E{E}, m_unitsX{unitsX}, m_unitsY{unitsY}
        {
            /*
                todo:
                check exceptions

            */

            auto index0{normalizationSpectrum.index(m_E)};

            m_dN0 = normalizationSpectrum.value(index0);

            auto E_left_0 = normalizationSpectrum.subintervalEnergyLowerBound(index0);
            auto E_right_0 = normalizationSpectrum.subintervalEnergyUpperBound(index0);

            m_dE0 = E_right_0 - E_left_0;
        }

        /***********************************************************************
        *              OTHERS
        ***********************************************************************/

        /**
         * @brief Destroy the PFluxPerEnergyIntervalMaker object
         * 
         */
        virtual ~PFluxPerEnergyIntervalMaker() {}

        /**
         * @brief Get a flux per energyIntervals interval built from
         * the given spectrum
         * 
         * @param spectrum this spectrum will be use to build the flux per dE
         * @param dA cross area of the flux per dE
         * @return PFluxPerEnergyInterval<energyIntervals> resulting flux per dE
         */
        PFluxPerEnergyInterval<energyIntervals>
        operator()(const PSpectrum<energyIntervals> &spectrum,
                   phys_float dA)
        {
            PFluxPerEnergyInterval<energyIntervals> flux_dE;
            for (phys_size i = 0; i < energyIntervals; i++)
            {
                auto E_i = spectrum.energy(i);

                flux_dE.m_data[i].first = E_i;

                auto dA0_dA = m_dA0 / dA;
                auto Ei_E = E_i / m_E;
                auto dNi_dN0 = spectrum.value(i) / m_dN0;

                auto E_i_left = spectrum.subintervalEnergyLowerBound(i);
                auto E_i_right = spectrum.subintervalEnergyUpperBound(i);
                auto dE_i = E_i_right - E_i_left;

                auto dE0_dEi = m_dE0 / dE_i;

                flux_dE.m_data[i].second = m_eta * dA0_dA * Ei_E * dNi_dN0 * dE0_dEi;
            }

            return std::move(flux_dE);
        }
    };

} // namespace physapi
