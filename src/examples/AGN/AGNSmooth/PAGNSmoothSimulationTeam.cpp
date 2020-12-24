#include"PAGNSmoothSimulationTeam.hpp"

namespace agn
{
	PAGNSmoothSimulationTeam::PAGNSmoothSimulationTeam(
		const std::shared_ptr<PAGNStructureModelB> structureModel,
		const std::shared_ptr<PVernerTable1> vernerTable1,
		const std::shared_ptr<PVernerTable2> vernerTable2,
		const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
		const std::shared_ptr<PAbundanceTable> abundances,
		phys_size id, phys_float numOfPhotons,
		const std::shared_ptr<PAGNFormula> agnformula)
		: PAGNSimulationTeamB(vernerTable1, vernerTable2, fluorescenceTable, abundances, id, numOfPhotons, agnformula),
		m_structureModel{ static_cast<const PAGNSmoothStructureModelB&>(*structureModel) }
	{
	}

	void PAGNSmoothSimulationTeam::run(std::string_view pathToStorageFolder)
	{
	}
}// namespace agn