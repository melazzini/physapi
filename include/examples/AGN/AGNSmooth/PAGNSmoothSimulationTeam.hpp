#pragma once
#include"PAGNSimulationTeamB.hpp"
#include"PAGNSmoothStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
namespace agn
{
	class PAGNSmoothSimulationTeam : public PAGNSimulationTeamB
	{
	public:
		PAGNSmoothSimulationTeam(
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
		const PAGNSmoothStructureModelB& m_structureModel;
	};
}// namespace agn