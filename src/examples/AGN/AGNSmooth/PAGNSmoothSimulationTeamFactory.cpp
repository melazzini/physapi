#include "PAGNSmoothSimulationTeamFactory.hpp"
#include "PAGNSmoothSimulationTeam.hpp"
namespace agn
{
	std::shared_ptr<PAGNSimulationTeam> agn::PAGNSmoothSimulationTeamFactory::
		buildSimulationTeam(
			const std::shared_ptr<PAGNStructureModelB> structureModel,
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons,
			const std::shared_ptr<PAGNFormula> agnformula)
	{
		auto team = new PAGNSmoothSimulationTeam(structureModel, vernerTable1, vernerTable2, fluorescenceTable, abundances, id, numOfPhotons, agnformula);

		std::shared_ptr<PAGNSmoothSimulationTeam> mySharedPtrToTeam(team);

		return mySharedPtrToTeam;
	}
}// namespace agn
