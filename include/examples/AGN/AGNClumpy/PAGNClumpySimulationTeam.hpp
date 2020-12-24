#pragma once

#include"PAGNSimulationTeamB.hpp"
#include"PAGNClumpyStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"

namespace agn
{
	class PAGNClumpySimulationTeam: public PAGNSimulationTeamB
	{
	public:
		PAGNClumpySimulationTeam(
			const std::shared_ptr<PAGNStructureModelB> structureModel,
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons,
			const std::shared_ptr<PAGNFormula> agnformula);

		// Inherited via PAGNSimulationTeam
		virtual void run(std::string_view pathToStorageFolder) override;

	private:
		const PAGNClumpyStructureModelB& m_structureModel;
	};
}// namespace agn