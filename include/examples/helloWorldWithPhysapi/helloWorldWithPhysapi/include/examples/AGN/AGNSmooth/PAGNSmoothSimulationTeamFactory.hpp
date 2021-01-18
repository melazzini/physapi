#pragma once

#include"PAGNSimulationTeamFactory.hpp"

namespace agn
{
	class PAGNSmoothSimulationTeamFactory :public PAGNSimulationTeamFactory
	{
	public:

		// Inherited via PAGNSimulationTeamFactory
		virtual std::shared_ptr<PAGNSimulationTeamB> buildSimulationTeam(
			const std::shared_ptr<PAGNStructureModelB> structureModel,
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter) override;

	};
}// namespace agn