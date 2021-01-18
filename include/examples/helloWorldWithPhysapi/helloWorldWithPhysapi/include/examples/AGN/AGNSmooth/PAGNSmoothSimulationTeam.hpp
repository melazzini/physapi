#pragma once
#include"PAGNSimulationTeamB.hpp"
#include"PAGNSmoothStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
#include"PAGNInitSpectrumDirectionFilter.hpp"
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
			phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter);

	private:
		const PAGNSmoothStructureModelB& m_structureModel;

	protected:

		// move the photon inside the agn internal structure and get the distance to
		// closest external boundary
		virtual std::optional<phys_float> distanceToBoundary(PSimplePhoton& photon)override;
	};
}// namespace agn