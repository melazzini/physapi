/*****************************************************************/ /**
 * \file   agn_utils.hpp
 * \brief  This file contains all the utilities needed by the agn
 *		   x-ray spectrum models, provided in this application module.
 *
 * \author francisco melazzini
 * \date   December 2020
 *********************************************************************/

/**
  * @defgroup examples
  *
  * @brief This module offers different ready-to-use examples.
  *
  */

/**
   * @defgroup agn
   *
   * @brief This module implements a model of an Active Galactic Nuclei X-Ray Spectral Model.
   *
   * @ingroup examples
   * 
   */

#pragma once

// #define PHYSAPI_USE_GPU

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
#include "verner_utils.hpp"
#include "PFluxPerEnergyIntervalMaker.hpp"

namespace agn
{
	using namespace physapi;

	/**
	 * @brief NUMBER OF COLUMNS IN THE AGN SIMULATION RESULTING DATA FILES
	 *
	 * @ingroup agn
	 */
	constexpr phys_size AGN_DATA_COLS = 5;

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
		INTRINSIC = 0,				 // PHOTON COMES DIRECTLY FROM THE AGN SOURCE, WITHOUT ANY INTERACTION AND WITHOUT ENTERING THE INTERNAL STRUCTURE OF THE AGN
		ENTEREDINTERNALGEOMETRY = 1, // THE PHOTON ENTERED THE INTERNAL STRUCTURE OF THE AGN, BUT IT DID NOT INTERACTED WITH MATTER INSIDE THE AGN
		REFLECTEDONLY = 2,			 // REPROCESSED PHOTON(ANY KIND OF INTERACTION BUT FLUORESCENCE)
		FLUORESCENT = 3,			 // SPECIFICALLY FLUORESCENCE
	};

	/**
	 * @brief Description of the agn resulting data from simulation.
	 *
	 * @ingroup agn
	 *
	 */
	enum class eAGNData
	{
		ENERGY = 0, // FIRST COLUMN IS THE PHOTON ENERGY
		THETA = 1,	// SECOND COLUMN IS THE POTHON'S DIRECTION THETA ANGLE(SPHERICAL COORDINATES)
		PHY = 2,	// SECOND COLUMN IS THE POTHON'S DIRECTION PHI ANGLE (SPHERICAL COORDINATES)
		TYPE = 3,	// TYPE OF PHOTON(DIRECT FROM THE AGN CENTER, SCATTERED, ETC. SEE @TYPEOFAGNPHOTONS)
		LINE = 4	// TYPE OF FLUORESCENCE LINE(I.E. NONE, FeKalpha, CaKalpha, ETC), SEE @eImportantFluorescentLines
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
		AlKalpha = 7,
		SiKalpha = 8,
		SKalpha = 9,
		ArKalpha = 10,
		CaKalpha = 11,
		CrKalpha = 12,
		FeKalpha = 13,
		NiKalpha = 14
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

	template <phys_size N_intervals_>
	using agnSpectrumList_t = std::map<phys_size, std::unique_ptr<PSpectrum<N_intervals_>>>;

	template <phys_size N_intervals_>
	using agnFlLinesSpectrumList_t = std::map<eImportantFluorescentLines, agnSpectrumList_t<N_intervals_>>;

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
	inline phys_float agnSolidAngle(phys_float zenith, phys_float dz)
	{
		return 2 * (solidAngle(zenith + dz) - solidAngle(zenith));
	}

	/**
	 * @brief This class represents an agn spectrum under a specific zenith angle
	 * and angular interval.
	 *
	 * To load the spectrum you can use physapi::loadFromFile()
	 *
	 * @ingroup agn
	 *
	 */
	template <phys_size N_intervals>
	class PAGNSpectrumZenith : public PSpectrumZenith<N_intervals, AGN_DATA_COLS>
	{
	public:
		/**
		 * @brief Build an empty spectrum.
		 * 
		 */
		PAGNSpectrumZenith()
			: PSpectrumZenith<N_intervals, AGN_DATA_COLS>(0, 0, 0, eSpectrumScale::LIN, 0)
		{
		}

		/**
		 * @brief Construct a new agn spectrum under a specfied zenith angle
		 *
		 * @param zenithAngle zenith angle (\f$ \theta \f$)
		 * @param E_low energy lower bound
		 * @param E_upp energy upper bound
		 * @param scale scale(logarithmic or linear)
		 * @param angularInterval angular interval size (\f$ \Delta \theta \f$)
		 */
		PAGNSpectrumZenith(phys_float zenithAngle,
						   phys_float E_low,
						   phys_float E_upp,
						   eSpectrumScale scale = eSpectrumScale::LIN,
						   phys_float angularInterval = 10.0_deg)
			: PSpectrumZenith<N_intervals, AGN_DATA_COLS>(zenithAngle, E_low, E_upp, scale, angularInterval)
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
			if (checkRangeInclusive(PSpectrumZenith<N_intervals, AGN_DATA_COLS>::minPhi(), std::abs(phi), PSpectrumZenith<N_intervals, AGN_DATA_COLS>::maxPhi()))
			{
				PSpectrumZenith<N_intervals, AGN_DATA_COLS>::addPhoton(row[static_cast<phys_size>(eAGNData::ENERGY)]);
			}
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
	template <phys_size N_intervals>
	class PAGNTransmittedSpectrumZenith : public PSpectrumZenith<N_intervals, AGN_DATA_COLS>
	{
	public:
		/**
		 * @brief Build an empty spectrum.
		 *
		 */
		PAGNTransmittedSpectrumZenith()
			: PSpectrumZenith<N_intervals, AGN_DATA_COLS>(0, 0, 0, eSpectrumScale::LIN, 0)
		{
		}

		/**
		 * @brief Construct a new agn spectrum under a specfied zenith angle
		 *
		 * @param zenithAngle zenith angle (\f$ \theta \f$)
		 * @param E_low energy lower bound
		 * @param E_upp energy upper bound
		 * @param scale scale(logarithmic or linear)
		 * @param angularInterval angular interval size (\f$ \Delta \theta \f$)
		 */
		PAGNTransmittedSpectrumZenith(phys_float zenithAngle,
									  phys_float E_low,
									  phys_float E_upp,
									  eSpectrumScale scale = eSpectrumScale::LIN,
									  phys_float angularInterval = 10.0_deg)
			: PSpectrumZenith<N_intervals, AGN_DATA_COLS>(zenithAngle, E_low, E_upp, scale, angularInterval)
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
			auto type = static_cast<eTypeOfAGNPhoton>(row[static_cast<phys_size>(eAGNData::TYPE)]);
			if (type == eTypeOfAGNPhoton::INTRINSIC || type == eTypeOfAGNPhoton::ENTEREDINTERNALGEOMETRY)
			{
				phys_float phi = row[static_cast<phys_size>(eAGNData::PHY)];
				if (checkRangeInclusive(PSpectrumZenith<N_intervals, AGN_DATA_COLS>::minPhi(), std::abs(phi), PSpectrumZenith<N_intervals, AGN_DATA_COLS>::maxPhi()))
				{
					PSpectrumZenith<N_intervals, AGN_DATA_COLS>::addPhoton(row[static_cast<phys_size>(eAGNData::ENERGY)]);
				}
			}
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
	template <phys_size N_intervals>
	class PAGNReflectedSpectrumZenith : public PSpectrumZenith<N_intervals, AGN_DATA_COLS>
	{
	public:
		/**
		 * @brief Build an empty spectrum.
		 *
		 */
		PAGNReflectedSpectrumZenith()
			: PSpectrumZenith<N_intervals, AGN_DATA_COLS>(0, 0, 0, eSpectrumScale::LIN, 0)
		{
		}

		/**
		 * @brief Construct a new agn spectrum under a specfied zenith angle
		 *
		 * @param zenithAngle zenith angle (\f$ \theta \f$)
		 * @param E_low energy lower bound
		 * @param E_upp energy upper bound
		 * @param scale scale(logarithmic or linear)
		 * @param angularInterval angular interval size (\f$ \Delta \theta \f$)
		 */
		PAGNReflectedSpectrumZenith(phys_float zenithAngle,
									phys_float E_low,
									phys_float E_upp,
									eSpectrumScale scale = eSpectrumScale::LIN,
									phys_float angularInterval = 10.0_deg)
			: PSpectrumZenith<N_intervals, AGN_DATA_COLS>(zenithAngle, E_low, E_upp, scale, angularInterval)
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
			auto type = static_cast<eTypeOfAGNPhoton>(row[static_cast<phys_size>(eAGNData::TYPE)]);
			if (type == eTypeOfAGNPhoton::REFLECTEDONLY)
			{
				phys_float phi = row[static_cast<phys_size>(eAGNData::PHY)];
				if (checkRangeInclusive(PSpectrumZenith<N_intervals, AGN_DATA_COLS>::minPhi(), std::abs(phi), PSpectrumZenith<N_intervals, AGN_DATA_COLS>::maxPhi()))
				{
					PSpectrumZenith<N_intervals, AGN_DATA_COLS>::addPhoton(row[static_cast<phys_size>(eAGNData::ENERGY)]);
				}
			}
		}
	};

	using t_clouds = std::vector<PPosition>;
	using t_histogram = std::map<phys_size, phys_float>;

	void loadClouds(t_clouds &clouds, const std::string &cloudsFile);

	void loadHistogram(t_histogram &histogram, const std::string &histogramFile);

	struct sPosition
	{
		phys_float x, y, z;
	};

	void printAGNClumpyTorusInfo(phys_float R1,
								 phys_float R2,
								 phys_float theta0,
								 phys_float N_H,
								 phys_float N_aver,
								 phys_float phi,
								 phys_size numOfPhotons,
								 phys_size numOfClouds,
								 phys_float T_e,
								 phys_float R_clouds,
								 std::ostream &os);

	/**
	 * @brief Get the T-spectrum type under dT-zenith angle interval
	 * 
	 * @tparam T_spectrum T_spectrum spectrum type
	 * @param data_files input data files
	 * @param theta zenith angle
	 * @param dTheta zenith angular interval
	 * @param E_low energy lower bound
	 * @param E_upp energy upper bound
	 * @return auto corresponding spectrum
	 */
	template <typename T_spectrum>
	auto get_T_dT_AGNSpectrum(const std::vector<std::string> &data_files, phys_float theta,
							  phys_float dTheta,
							  phys_float E_low, phys_float E_upp)
	{
		auto T_dT_Spectrum = T_spectrum(theta,
										E_low, E_upp,
										eSpectrumScale::LOG,
										dTheta);
		for (auto &&file : data_files)
		{
			loadFromFile<AGN_DATA_COLS>(file, T_dT_Spectrum);
		}

		return T_dT_Spectrum;
	}

	/**
	 * @brief Process and print the spectrum of type T_spectrum under dT-zenith
	 * 
	 * @tparam T_spectrum spectrum type
	 * @param data_files input data files
	 * @param outputFileName output file name
	 * @param theta zenith angle
	 * @param dTheta zenith angular interval
	 * @param E_low energy lower bound
	 * @param E_upp energy upper bound
	 */
	template <typename T_spectrum>
	void process_T_dT_AGNSpectrum(const std::vector<std::string> &data_files,
								  const std::string &outputFileName,
								  phys_float theta,
								  phys_float dTheta,
								  phys_float E_low, phys_float E_upp)
	{
		std::ofstream fout;

		fout.open(outputFileName);

		auto &&T_dT_Spectrum = std::move(get_T_dT_AGNSpectrum<T_spectrum>(data_files, theta, dTheta, E_low, E_upp));

		fout << T_dT_Spectrum << std::endl;
	}

} // namespace agn
