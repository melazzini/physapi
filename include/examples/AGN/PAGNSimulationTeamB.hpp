#pragma once

#include"agn_utils.hpp"
#include <string_view>
#include"PAGNSmoothStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
#include"PAGNInitSpectrumDirectionFilter.hpp"
namespace agn
{
	/**
	 * @brief This class represents a team of objects that handle the AGN X-Ray simulation.
	 * 
	 * This class is like an office where you have different workers, where each handles a
	 * specific task, and all work together in order to simulate the x-ray spectrum of the
	 * given agn. For example: one object handles the calculation of the photo-ionization
	 * cross section, other the fluorescence, there is one object, which tracks each photon
	 * on its trajectory until it gets registered after leaving the agn, etc.
	 * 
	 * This class provides a simple tracking predefined algorithm to simulate x-ray spectrum
	 * for a given agn, with an arbitrary internal structure as well as an arbitrary geometric shape.
	 * To be able to use this class, you only need to derive from it a child class, and override
	 * the protected method distanceToBoundary(&photon), which receives a photon and returns the
	 * distance to the next escaping boundary on the photon's path, see distanceToBoundary().
	 * 
	 * @ingroup agn
	 * 
	 */
	class PAGNSimulationTeamB
	{
	private:

#ifdef AGN_INTERVALS
		static constexpr phys_size N_intervals = AGN_INTERVALS;
#else
		static constexpr phys_size N_intervals = 2000;

#endif // AGN_INTERVALS

#ifdef AGN_SPECTRUM_PRECISION
		static constexpr phys_size SPECTRUM_PRECISION = AGN_SPECTRUM_PRECISION;
#else
		static constexpr phys_size SPECTRUM_PRECISION = 10'000'000; // how close to the exact number of photons in the simulation
#endif // AGN_SPECTRUM_PRECISION

		// describes the list(vector) of absorbing elements
		enum eAbsorbingElementsDescription
		{
			Z = 0,    // element's atomic number
			Ne = 1,   // element's number of electrons
			IS = 2,   // element's shell number
			SIGMA = 3 // element's cross section
		};

	public:

		/**
		 * @brief Creates a simulation team object.
		 * 
		 * \param vernerTable1
		 * \param vernerTable2
		 * \param fluorescenceTable
		 * \param abundances
		 * \param id
		 * \param numOfPhotons
		 * \param n_e
		 * \param T_e
		 * \param agnformula
		 * \param initSpectrumDirFilter
		 */
		PAGNSimulationTeamB(
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter
		);

		/**
		 * @brief Run the simulation.
		 * 
		 * \param pathToStorageFolder
		 */
		virtual void run(std::string_view pathToStorageFolder);

		/**
		 * @brief Get the team's id.
		 *
		 * \return team's id
		 */
		phys_size id()const { return m_id; }

		/**
		 * @brief Get the number of photons in the current team.
		 *
		 * \return the number of photons in the current team
		 */
		phys_float numOfPhotons()const { return m_numOfPhotons; }

	protected:

		/**
		 * @brief move the photon inside the agn internal structure and get the distance to
		 * closest external boundary.
		 * 
		 * \param photon
		 * \return 
		 */
		virtual std::optional<phys_float> distanceToBoundary(PSimplePhoton& photon) = 0;

	private:
		const std::shared_ptr<PVernerTable1> m_vernerTable1;
		const std::shared_ptr<PVernerTable2> m_vernerTable2;
		const std::shared_ptr<PFluorescenceTable> m_fluorescenceTable;
		const std::shared_ptr<PAbundanceTable> m_abundances;
		phys_size m_id; // id of the current team
		phys_float m_numOfPhotons; // number of photons in the current team
		phys_float m_n_e; // concentration of electrons
		phys_float m_T_e; // temperature of the electrons
		const std::shared_ptr<PAGNFormula> m_formula; // agn spectrum formula
	private:
		PVerner m_verner; // calculates the photoionization cross section
		PFluorescenceHnd m_fluorescenceHnd; // handles fluorescence simulation
		PAGNSimulationMng m_simMng; // simulation manager
		PSpectrum<N_intervals> m_initSpectrum; // initial spectrum from the agn source(SMBH at the center)
		PRandom m_randomValuesMng; // random values generator
		PRandomDirection m_randDirMng; // // random directions generator
		const PAGNInitSpectrumDirectionFilter& m_dirFilter; // selects the allowed directions into the agn for the initial photons
		PComptonCollider<physapi::PMCCompton> m_comptonHnd; // this handles compton simulation
	private:
		phys_float m_sigmaVerner; // photonionization cross section
		std::vector<std::array<phys_float, ABSORBINGELEMENT_PROPERTIES>> m_listOfAbsorbingElements;
		phys_float m_sigmaKleinNishina; // Klein Nishina cross section
		phys_float m_sigmaTotal; // total cross section
		phys_float m_lambdaMean; // mean free path
		phys_float m_escapeProbability;

	private:
		// get the fluorescent line, if any occurs, for the given arguments
		std::optional<t_fluorescentLine>
			fluorescentLine(const std::vector<std::array<phys_float, ABSORBINGELEMENT_PROPERTIES>>& absorbingElements,
				phys_float sigmaVerner);

		// get the mean free path for the given total cross section
		phys_float lambdaMean(phys_float sigmaTotal)
		{
			return (sigmaTotal > 0) ? (1.0 / (m_n_e * m_sigmaTotal)) : PLUS_INF;
		}

		// get the escape probability
		phys_float escapeProbability(phys_float distanceToBoundary, phys_float lambdaMean)
		{
			return (lambdaMean < PLUS_INF) ? std::exp(-distanceToBoundary / lambdaMean) : 1;
		}
	};
}// namespace agn