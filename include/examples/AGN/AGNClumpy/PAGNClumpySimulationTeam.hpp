#pragma once

#include"PAGNSimulationTeamB.hpp"
#include"PAGNClumpyStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
#include<optional>
#include"PAGNClumpyCloudFinder.hpp"

namespace agn
{
	class PAGNClumpySimulationTeam : public PAGNSimulationTeamB
	{
	public:
		PAGNClumpySimulationTeam(
			const std::shared_ptr<PAGNStructureModelB> structureModel,
			const std::shared_ptr<PVernerTable1> vernerTable1,
			const std::shared_ptr<PVernerTable2> vernerTable2,
			const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
			const std::shared_ptr<PAbundanceTable> abundances,
			phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter);

	private:
		const PAGNClumpyStructureModelB& m_structureModel;
		std::optional<phys_size> m_cloudIndex; // last cloud index, for efficiency
		PSphere m_cloud;
		PAGNClumpyCloudFinder m_cloudFinder;

	protected:
		// move the photon inside the agn internal structure and get the distance to
		// closest external boundary
		virtual std::optional<phys_float> distanceToBoundary(PSimplePhoton& photon)override;
	};
}// namespace agn