#pragma once

#include"agn_utils.hpp"
#include <string_view>
#include"PAGNSmoothStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
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

	public:
		PAGNSimulationTeamB(
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons,
			const std::shared_ptr<PAGNFormula> agnformula
		);

		virtual void run(std::string_view pathToStorageFolder) = 0;

	private:
		const std::shared_ptr<PVernerTable1> m_vernerTable1;
		const std::shared_ptr<PVernerTable2> m_vernerTable2;
		const std::shared_ptr<PFluorescenceTable> m_fluorescenceTable;
		const std::shared_ptr<PAbundanceTable> m_abundances;
		phys_size m_id;
		phys_float m_numOfPhotons;
		const std::shared_ptr<PAGNFormula> m_formula;
	protected:
		PVerner m_verner;
		PFluorescenceHnd m_fluorescenceHnd;
		PAGNSimulationMng m_simMng;
		PSpectrum<N_intervals> m_initSpectrum;
	};
}// namespace agn