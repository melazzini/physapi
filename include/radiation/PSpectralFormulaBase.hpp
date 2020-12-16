#pragma once
#include "radiation_utils.hpp"

namespace physapi
{   
    /**
     * @brief This is the base class for spectral formulas or functions.
     * 
     * Derive your own formulas from this class in order to create 
     * different kind of spectra with PSpectrumMaker.
     * 
     * @see PSpectrumMaker
     * 
     * @ingroup radiation
     * 
     */
    class PSpectralFormulaBase
    {
    protected:
        /**
         * @brief Construct a new PSpectralFormulaBase object
         * 
         */
        explicit PSpectralFormulaBase() {}

    public:
        /**
         * @brief Destroy the PSpectralFormulaBase object
         * 
         */
        virtual ~PSpectralFormulaBase() {}

        /**
         * @brief Get the value of the spectral formula for the corresponding energy
         * 
         * @param energy 
         * @return phys_float value of the formula
         */
        virtual phys_float operator()(phys_float energy) const = 0;
    };
} // namespace physapi
