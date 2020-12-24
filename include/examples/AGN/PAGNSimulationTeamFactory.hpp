#pragma once

#include "PAGNSimulationTeam.hpp"
#include"PAGNStructureModelB.hpp"
#include"verner_utils.hpp"
#include"PFluorescenceTable.hpp"
#include <memory>
#include <string_view>
#include"PAGNFormula.hpp"

namespace agn
{
	class PAGNSimulationTeamFactory
	{
	public:
		virtual std::shared_ptr<PAGNSimulationTeam> 
			buildSimulationTeam(const std::shared_ptr<PAGNStructureModelB>structureModel,
				const std::shared_ptr<PVernerTable1>vernerTable1,
				const std::shared_ptr<PVernerTable2>vernerTable2,
				const std::shared_ptr<PFluorescenceTable>fluorescenceTable, 
				const std::shared_ptr<PAbundanceTable> abundances,
				phys_size id,
				phys_float numOfPhotons,
				const std::shared_ptr<PAGNFormula> agnformula) = 0;
	};
}