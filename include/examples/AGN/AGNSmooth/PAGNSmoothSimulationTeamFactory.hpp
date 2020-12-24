#pragma once

#include"PAGNSimulationTeamFactory.hpp"
#include"PAGNFormula.hpp"
namespace agn
{
	class PAGNSmoothSimulationTeamFactory :public PAGNSimulationTeamFactory
	{
	public:

		// Inherited via PAGNSimulationTeamFactory
		virtual std::shared_ptr<PAGNSimulationTeam> buildSimulationTeam(
			const std::shared_ptr<PAGNStructureModelB> structureModel,
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons,
			const std::shared_ptr<PAGNFormula> agnformula) override;

	};
}// namespace agn