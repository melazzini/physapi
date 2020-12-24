#include "PAGNSmoothSimulationTeamFactory.hpp"
#include "PAGNSmoothSimulationTeam.hpp"
#include "PAGNInitSpectrumDirectionFilter.hpp"
namespace agn
{
	std::shared_ptr<PAGNSimulationTeamB> agn::PAGNSmoothSimulationTeamFactory::
		buildSimulationTeam(
			const std::shared_ptr<PAGNStructureModelB> structureModel,
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons, phys_float n_e,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter)
	{
		auto team = new PAGNSmoothSimulationTeam(structureModel, 
			vernerTable1, 
			vernerTable2, 
			fluorescenceTable, 
			abundances, id, numOfPhotons, n_e,
			agnformula, initSpectrumDirFilter);

		std::shared_ptr<PAGNSmoothSimulationTeam> mySharedPtrToTeam(team);

		return mySharedPtrToTeam;
	}
}// namespace agn
