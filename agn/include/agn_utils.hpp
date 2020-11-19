#pragma once

#include "PSpectrumMaker.hpp"
#include "PGeometryBase.hpp"
#include "PSphere.hpp"
#include "PSimpleTorus.hpp"
#include "PAbundanceTable.hpp"
#include "PFluorescenceHnd.hpp"
#include "PVerner.hpp"
#include "PComptonCollider.hpp"
#include "PMCCompton.hpp"
#include "PRandomDirection.hpp"
#include "compton_utils.hpp"
#include "montecarlo_utils.hpp"
#include "PSpectrumZenith.hpp"

/**
 * @defgroup examples
 * 
 * @brief Here you can see different examples using physapi.
 * 
 */

/**
 * @defgroup agn
 * 
 * @brief This group shows how to simulate active galactic nuclei spectrum.
 * 
 * @ingroup examples
 * 
 */

namespace agn
{
    using physapi::cub;
    using physapi::PGeometryBase;
    using physapi::phys_float;
    using physapi::phys_int;
    using physapi::PPosition;
    using physapi::PSimpleTorus;
    using physapi::PSpectrum;
    using physapi::PSpectrumMaker;
    using physapi::PSphere;
    using physapi::operator"" _m;
    using physapi::operator"" _s;
    using physapi::operator"" _kg;
    using physapi::Pi;
    using physapi::operator"" _deg;
    using physapi::operator"" _cm;
    using physapi::operator"" _eVToKelvin;
    using physapi::operator"" _cm2;
    using physapi::operator"" _eV;
    using physapi::PAbundanceTable;
    using physapi::PFluorescenceHnd;
    using physapi::phys_size;
    using physapi::PSpectralFormulaBase;
    using physapi::PVerner;
    using PComptonHnd = physapi::PComptonCollider<physapi::PMCCompton>;
    using physapi::checkRangeInclusive;
    using physapi::eSpectrumScale;
    using physapi::eZ;
    using physapi::getRandomIndexMontecarlo;
    using physapi::KleinNishina;
    using physapi::PFluorescenceKey;
    using physapi::phys_bool;
    using physapi::PLUS_INF;
    using physapi::PRandom;
    using physapi::PRandomDirection;
    using physapi::PSimplePhoton;
    using physapi::PSpectrumZenith;
    using physapi::PVector3D;
    using physapi::solidAngle;
    using physapi::t_fluorescentLine;
    using physapi::operator"" _m;
    using physapi::eNumOfThreads;

    /************************************************************************
     * AGN USER PARAMETER DEFINITIONS BEGIN
    ************************************************************************/

    constexpr phys_float SPHERE_R = 1.E15_cm;       // sphere radius
    constexpr phys_float TORUS_R1 = 1.E14_cm;       // torus internal radius
    constexpr phys_float TORUS_R2 = 1.E15_cm;       // torus external radius
    constexpr phys_float TORUS_THETA0 = 60.0_deg;   // torus half opening angle
    constexpr phys_float N_H = 1.0E23 / 1.0_cm2;    // average column density
    constexpr phys_float T_e = 1.0_eVToKelvin;      // temperature of electrons
    constexpr phys_float NUMBER_OF_PHOTONS = 3.0E6; // NUMBER OF PHOTONS IN THE SIMULATION
    const std::string PATH_STORAGE_DATA = "./data/";

    /**
     * @brief Determine what directions are valid when creating the photons
     * 
     * @ingroup agn
     * 
     */
    struct DirectionPredicate
    {
        static phys_bool directionOk(const PVector3D &direction)
        {
            // this is for the simple torus
            return std::abs(direction.phi()) <= (Pi / 2 - TORUS_THETA0);

            // this is for the sphere
            // return true;
        }
    };

    /**
     * @brief This class represents an active galactic nuclei 
     * intrinsic spectrum formula. 
     * 
     * Modify this class in if you need other kind of initial spectrum.
     * 
     * @ingroup agn
     * 
     */
    class AGNFormula : public PSpectralFormulaBase
    {
    public:
        /**
         * @brief FULL ENERGY INTERVAL UPPER BOUND OR CUT-OFF ENERGY
         * 
         */
        constexpr static phys_float E_cut = 300.0E3;

        /**
         * @brief FULL ENERGY INTERVAL LOWER BOUND
         * 
         */
        constexpr static phys_float E_low = 100.0;

        /**
         * @brief GAMMA PARAMETER OF THE AGN FORMULA
         * 
         */
        constexpr static phys_float gamma = 1.8;

        /**
         * @brief NUMBER OF ENERGY SUBINTERVALS, OR JUST INTERVALS FOR SHORT
         * 
         */
        constexpr static phys_size N_intervals = 2000;

        /**
         * @brief Precision in the generation of the initial agn spectrum.
         * 
         * This parameter influences the number of photons generated in
         * the initial spectrum.
         * 
         * The higher the value of this parameter the longer the time needed
         * to generate the initial spectrum.
         * 
         */
        constexpr static phys_size SPECTRUM_PRECISION = 2'000'000;

        /**
         * @brief Formula for agn spectrum
         * 
         * @param energy 
         * @return phys_float 
         */
        phys_float operator()(phys_float energy) const override
        {
            return std::pow(energy, -gamma) * std::exp(-energy / E_cut);
        }
    };

    /************************************************************************
     * AGN USER PARAMETER DEFINITIONS END
    ************************************************************************/

    /************************************************************************
     * AGN NOT USER PARAMETER DEFINITIONS BEGIN
    ************************************************************************/

    constexpr phys_float TOT_NUMBER_OF_PHOTONS = -1; // TOTAL NUM OF PHOTONS INCLUDED THOSE
                                                     //WHICH ARE NOT SIMULATED(DID NOT ENTER THE TORUS)

    /**
     * @brief NUMBER OF COLUMNS IN THE AGN SIMULATION RESULTING DATA FILES
     * 
     * @ingroup agn
     */
    constexpr phys_size AGN_DATA_COLS = 5;

    /**
     * @brief Description of the agn resulting data from simulation.
     * 
     * @ingroup agn
     * 
     */
    enum class eAGNData
    {
        ENERGY = 0, // FIRST COLUMN IS THE PHOTON ENERGY
        THETA = 1,  // SECOND COLUMN IS THE POTHON'S DIRECTION THETA ANGLE(SPHERICAL COORDINATES)
        PHY = 2,    // SECOND COLUMN IS THE POTHON'S DIRECTION PHI ANGLE (SPHERICAL COORDINATES)
        TYPE = 3,   // TYPE OF PHOTON(DIRECT FROM THE AGN CENTER, SCATTERED, ETC. SEE @TYPEOFAGNPHOTONS)
        LINE = 4    // TYPE OF FLUORESCENCE LINE(I.E. NONE, FeKalpha, CaKalpha, ETC), SEE @eImportantFluorescentLines
    };

    /**
     * @brief Type of agn photons from the simulation.
     * 
     * @TYPEOFAGNPHOTONS
     * 
     * @ingroup agn
     * 
     */
    enum class eTypeOfAGNPhoton
    {
        INTRINSIC = 0,   // PHOTON COMES DIRECTLY FROM THE AGN SOURCE, WITHOUT ANY INTERACTION
        REPROCESSED = 1, // REPROCESSED PHOTON(ANY KIND OF INTERACTION)
    };

    /**
     * @brief Type of important fluorescent lines
     * 
     * @eImportantFluorescentLines
     * 
     * @ingroup agn
     * 
     */
    enum class eImportantFluorescentLines
    {
        NONE = 0,
        CKalpha,
        NKalpha,
        OKalpha,
        NeKalpha,
        NaKalpha,
        MgKalpha,
        AlKalpha,
        SiKalpha,
        SKalpha,
        ArKalpha,
        CaKalpha,
        CrKalpha,
        FeKalpha,
        NiKalpha
    };

    /**
     * @brief C K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey CKalpha1{eZ::C, 1, 1, 0, 1};

    /**
     * @brief C K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey CKalpha2{eZ::C, 1, 1, 0, 2};

    /**
     * @brief N K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NKalpha1{eZ::N, 1, 1, 0, 1};

    /**
     * @brief N K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NKalpha2{eZ::N, 1, 1, 0, 2};

    /**
     * @brief O K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey OKalpha1{eZ::O, 1, 1, 0, 1};

    /**
     * @brief O K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey OKalpha2{eZ::O, 1, 1, 0, 2};

    /**
     * @brief Ne K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NeKalpha1{eZ::Ne, 1, 1, 0, 1};

    /**
     * @brief Ne K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NeKalpha2{eZ::Ne, 1, 1, 0, 2};

    /**
     * @brief Na K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NaKalpha1{eZ::Na, 1, 1, 0, 1};

    /**
     * @brief Na K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NaKalpha2{eZ::Na, 1, 1, 0, 2};

    /**
     * @brief Mg K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey MgKalpha1{eZ::Mg, 1, 1, 0, 1};

    /**
     * @brief Mg K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey MgKalpha2{eZ::Mg, 1, 1, 0, 2};

    /**
     * @brief Al K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey AlKalpha1{eZ::Al, 1, 1, 0, 1};

    /**
     * @brief Al K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey AlKalpha2{eZ::Al, 1, 1, 0, 2};

    /**
     * @brief Si K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey SiKalpha1{eZ::Si, 1, 1, 0, 1};

    /**
     * @brief Si K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey SiKalpha2{eZ::Si, 1, 1, 0, 2};

    /**
     * @brief S K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey SKalpha1{eZ::S, 1, 1, 0, 1};

    /**
     * @brief S K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey SKalpha2{eZ::S, 1, 1, 0, 2};

    /**
     * @brief Ar K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey ArKalpha1{eZ::Ar, 1, 1, 0, 1};

    /**
     * @brief Ar K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey ArKalpha2{eZ::Ar, 1, 1, 0, 2};

    /**
     * @brief Ca K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey CaKalpha1{eZ::Ca, 1, 1, 0, 1};

    /**
     * @brief Ca K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey CaKalpha2{eZ::Ca, 1, 1, 0, 2};

    /**
     * @brief Cr K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey CrKalpha1{eZ::Cr, 1, 1, 0, 1};

    /**
     * @brief Cr K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey CrKalpha2{eZ::Cr, 1, 1, 0, 2};

    /**
     * @brief Fe K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey FeKalpha1{eZ::Fe, 1, 1, 0, 1};

    /**
     * @brief Fe K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey FeKalpha2{eZ::Fe, 1, 1, 0, 2};

    /**
     * @brief Ni K-alpha fluorescent line 1
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NiKalpha1{eZ::Ni, 1, 1, 0, 1};

    /**
     * @brief Ni K-alpha fluorescent line 2
     * 
     * @ingroup agn
     * 
     */
    const PFluorescenceKey NiKalpha2{eZ::Ni, 1, 1, 0, 2};

    /**
     * @brief type of resulting data from agn simulation
     * 
     * @ingroup agn
     * 
     */
    using t_resultingData = std::vector<std::array<phys_float, AGN_DATA_COLS>>;

    /**
     * @brief This functor filters the initial directions of the photons
     * generated in the simulation. This is a wrapper of 
     * DirectionPredicate::directionOk()
     * 
     * @ingroup agn
     * 
     */
    class DirectionFilter
    {

    public:
        phys_bool operator()(const PVector3D &direction) const
        {
            return DirectionPredicate::directionOk(direction);
        }
    };

    /**
     * @brief This class represents an agn spectrum under a specific zenith angle
     * and angular interval.
     * 
     * To load the spectrum you can use physapi::loadFromFile()
     * 
     * @ingroup agn
     * 
     */
    class AGNSpectrumZenith : public PSpectrumZenith<AGNFormula::N_intervals, AGN_DATA_COLS>
    {
    public:
        /**
         * @brief Construct a new agn spectrum under a specfied zenith angle
         * 
         * @param zenithAngle zenith angle (\f$ \theta \f$)
         * @param E_low energy lower bound
         * @param E_upp energy upper bound
         * @param scale scale(logarithmic or linear)
         * @param angularInterval angular interval size (\f$ \Delta \theta \f$)
         */
        AGNSpectrumZenith(phys_float zenithAngle,
                          phys_float E_low,
                          phys_float E_upp,
                          eSpectrumScale scale = eSpectrumScale::LIN,
                          phys_float angularInterval = 10.0_deg)
            : PSpectrumZenith(zenithAngle, E_low, E_upp, scale, angularInterval)
        {
        }

        /**
         * @brief count a photon into the spectrum
         * 
         * This method is meant to be used in conjunction with physapi::loadFromFile()
         * 
         * @param row 
         */
        virtual void loadElement(const std::array<phys_float, AGN_DATA_COLS> &row) override
        {
            phys_float phi = row[static_cast<phys_size>(eAGNData::PHY)];
            if (checkRangeInclusive(minPhi(), std::abs(phi), maxPhi()))
            {
                addPhoton(row[static_cast<phys_size>(eAGNData::ENERGY)]);
            }
        }
    };

    /**
     * @brief Get the total solid angle of an agn angular portion.
     * 
     * The total solid angle consists of both parts over the equatorial
     * horizontal plane and under the same plane.
     * The total solid angle is defined by the zenith angle \f$ \theta \f$
     * and the angular interval \f$ \delta \theta \f$. Therefore, if
     * for example the zenith is 0 and the angular interval Pi/2 then
     * you will get \f$ 4 \pi \f$.
     * 
     * @param zenith 
     * @param dz 
     * @return constexpr phys_float 
     * 
     * @ingroup agn
     */
    constexpr phys_float agnSolidAngle(phys_float zenith, phys_float dz)
    {
        return 2 * (solidAngle(zenith + dz) - solidAngle(zenith));
    }

    /************************************************************************
     * AGN NOT USER PARAMETER DEFINITIONS END
    ************************************************************************/

} // namespace agn
