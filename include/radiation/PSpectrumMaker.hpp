#pragma once
#include "PSpectrum.hpp"
#include "PSpectralFormulaBase.hpp"
#include <string>

namespace physapi
{
    /**
     * @brief This is a functor class for making radiation spectra.
     * 
     * @tparam energyIntervals number of energy intervals
     * 
     * @tparam precision precision in the generation of photons. The more precision
     *         the more exact number of photons you will obtain.
     * 
     * A radiation spectrum is defined in our api by the next formula:
     * 
     *      \f$ \frac{dN}{dE} = \beta * N_{Tot} * \phi(E) \f$
     * 
     * The formula gives the number of photons per energy interval corresponding
     * to the energy \f$ E \f$.
     * 
     * Where \f$ \beta \f$ is the normalization parameter, \f$ N_{Tot} \f$
     * the total amount of photons in the spectrum and \f$ \phi(E) \f$ is
     * the spectral formula.
     * 
     * We normalize this such that:
     * 
     *      \f$ \beta * \int_{E_{low}}^{E_{upp}} \phi(E) = 1 \f$
     * 
     * and therefore:
     *      
     *      \f$ \int_{E_{low}}^{E_{upp}} dN = N_{Tot} \f$
     * 
     * Where \f$ E_{low} \f$ and \f$ E_{upp} \f$ are the energy bounds
     * of the spectrum.
     * 
     * @see PSpectralFormulaBase
     * 
     * @ingroup radiation
     * 
     */
    template <phys_size energyIntervals, phys_size precision = 1'000'000>
    class PSpectrumMaker
    {
    private:
        std::string m_unitsX;

        std::string m_unitsY;

    public:
        /**
         * @brief Number of energy intervals.
         * 
         */
        static constexpr phys_size INTERVALS = energyIntervals;

        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new PSpectrumMaker object
         * 
         * @param unitsX units of the x axis
         * @param unitsY units of the y axis
         */
        PSpectrumMaker(const std::string &unitsX = "j", const std::string &unitsY = "adim")
            : m_unitsX{unitsX}, m_unitsY{unitsY}
        {
        }

        /**
         * @brief Destroy the PSpectrumMaker object
         * 
         */
        virtual ~PSpectrumMaker() {}

        /***********************************************************************
        *              OTHERS
        ***********************************************************************/

        /**
         * @brief Make the spectrum.
         * 
         * 
         * @tparam energyIntervals number of energy intervals
         * @param spectrum empty spectrum
         * @param numberOfPhotons total number of photons
         * @param phi spectral formula
         * 
         * @note The spectrum will be cleared befored it gets 
         * filled with the data.
         */
        void operator()(PSpectrum<energyIntervals> &spectrum,
                        phys_float numberOfPhotons,
                        const PSpectralFormulaBase &phi) const
        {
            phys_float beta{calculateBeta(spectrum, phi)};

            spectrum.clear();

            for (size_t i = 0; i < energyIntervals; i++)
            {
                auto E_i{spectrum.energy(i)};

                auto E_low_i{spectrum.subintervalEnergyLowerBound(i)};
                auto E_upp_i{spectrum.subintervalEnergyUpperBound(i)};

                auto dE_i{E_upp_i - E_low_i};

                auto phi_ = phi(E_i);

                spectrum.addPhotons(i,
                                    (beta * numberOfPhotons * phi_ * dE_i));
            }
        }

    private:
        phys_float calculateBeta(const PSpectrum<energyIntervals> &spectrum,
                                 const PSpectralFormulaBase &phi) const
        {
            phys_float spectralFormulaIntegral{integrate<const PSpectralFormulaBase &>(phi, spectrum.leftBound(), spectrum.rightBound(), precision)};

            return 1 / spectralFormulaIntegral;
        }
    };

} // namespace physapi
