#pragma once

#include "utils.hpp"
#include <tuple>

/**
 * @defgroup astrophysics
 * 
 * @brief This module offers tools for astrophysics.
 * 
 */

namespace physapi
{
    /**
     * @brief Atomic numbers enumeration
     * 
     * @ingroup astrophysics
     * 
     */
    enum class eZ
    {
        H = 1,
        He = 2,
        Li = 3,
        Be = 4,
        B = 5,
        C = 6,
        N = 7,
        O = 8,
        F = 9,
        Ne = 10,
        Na = 11,
        Mg = 12,
        Al = 13,
        Si = 14,
        P = 15,
        S = 16,
        Cl = 17,
        Ar = 18,
        K = 19,
        Ca = 20,
        Sc = 21,
        Ti = 22,
        V = 23,
        Cr = 24,
        Mn = 25,
        Fe = 26,
        Co = 27,
        Ni = 28,
        Cu = 29,
        Zn = 30
    };

    // NUMBER OF COLUMNS IN THE ABUNDANCE TABLE
    constexpr phys_size ABUNDANCE_TABLE_COLS = 3;

    // NUMBER OF COLUMNS IN THE FLUORESCENCE TABLE
    constexpr phys_size FLUORESCENCE_TABLE_COLS = 7;

    /**
     * @brief This key determines a fluorescence line
     * 
     * @ingroup astrophysics
     * 
     */
    using t_fluorescenceKey = std::array<phys_size, 5>;

    /**
     * @brief this is the value of a fluorescence line
     * 
     * @ingroup astrophysics
     * 
     */
    using t_fluorescenceValue = std::array<phys_float, 2>;

    class PFluorescenceValue : public std::array<phys_float, 2>
    {
    public:
        PFluorescenceValue()
        {
            (*this)[0] = 0;
            (*this)[1] = 0;
        }

        PFluorescenceValue(phys_float energy, phys_float yield)
        {
            (*this)[0] = energy;
            (*this)[1] = yield;
        }

        phys_float lineEnergy() const { return (*this)[0]; }

        phys_float lineYield() const { return (*this)[1]; }
    };

    class PFluorescenceKey : public std::array<phys_size, 5>
    {
    public:
        PFluorescenceKey()
        {
            (*this)[0] = 0;
            (*this)[1] = 0;
            (*this)[2] = 0;
            (*this)[3] = 0;
            (*this)[4] = 0;
        }

        PFluorescenceKey(eZ z, phys_size st, phys_size is, phys_size aug, phys_size lineNumber)
        {
            (*this)[0] = static_cast<phys_size>(z);
            (*this)[1] = st;
            (*this)[2] = is;
            (*this)[3] = aug;
            (*this)[4] = lineNumber;
        }

        eZ atomicNumber() const { return static_cast<eZ>((*this)[0]); }

        phys_size ionizationState() const { return (*this)[1]; }

        phys_size shellNUmber() const { return (*this)[2]; }

        phys_size augerElectrons() const { return (*this)[3]; }

        phys_size lineNumber() const { return (*this)[4]; }
    };

    /**
     * @brief Definition of a fluorescence line
     * 
     * @ingroup astrophysics
     * 
     */
    using t_fluorescentLine = std::pair<PFluorescenceKey, PFluorescenceValue>;

    std::ostream &operator<<(std::ostream &os, eZ z);

} // namespace physapi
