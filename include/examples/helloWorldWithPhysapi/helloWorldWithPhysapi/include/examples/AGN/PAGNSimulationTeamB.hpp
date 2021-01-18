#pragma once

#include"agn_utils.hpp"
#include <string_view>
#include"PAGNSmoothStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
#include"PAGNInitSpectrumDirectionFilter.hpp"
namespace agn
{
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
		static constexpr phys_size SPECTRUM_PRECISION = 10'000'000;
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
		PAGNSimulationTeamB(
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter
		);

		virtual void run(std::string_view pathToStorageFolder);

		/**
		 * Get the team's id.
		 *
		 * \return team's id
		 */
		phys_size id()const { return m_id; }

		/**
		 * Get the number of photons in the current team.
		 *
		 * \return the number of photons in the current team
		 */
		phys_float numOfPhotons()const { return m_numOfPhotons; }


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
	protected:
		PVerner m_verner; // calculates the photoionization cross section
		PFluorescenceHnd m_fluorescenceHnd; // handles fluorescence simulation
		PAGNSimulationMng m_simMng; // simulation manager
		PSpectrum<N_intervals> m_initSpectrum; // initial spectrum from the agn source(SMBH at the center)
		PRandom m_randomValuesMng; // random values generator
		PRandomDirection m_randDirMng; // // random directions generator
		const PAGNInitSpectrumDirectionFilter& m_dirFilter; // selects the allowed directions into the agn for the initial photons
		PComptonCollider<physapi::PMCCompton> m_comptonHnd; // this handles compton simulation
	protected:
		phys_float m_sigmaVerner; // photonionization cross section
		std::vector<std::array<phys_float, ABSORBINGELEMENT_PROPERTIES>> m_listOfAbsorbingElements;
		phys_float m_sigmaKleinNishina; // Klein Nishina cross section
		phys_float m_sigmaTotal; // total cross section
		phys_float m_lambdaMean; // mean free path
		phys_float m_escapeProbability;

	protected:
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

		// move the photon inside the agn internal structure and get the distance to
		// closest external boundary
		virtual std::optional<phys_float> distanceToBoundary(PSimplePhoton& photon) = 0;

		// get the fluorescent line, if any occurs, for the given arguments
		std::optional<t_fluorescentLine>
			fluorescentLine(const std::vector<std::array<phys_float, ABSORBINGELEMENT_PROPERTIES>>& absorbingElements,
				phys_float sigmaVerner);
	};
}// namespace agn