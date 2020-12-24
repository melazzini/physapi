#pragma once

#include"PAGNSimulationTeamB.hpp"
#include"PAGNClumpyStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
#include<optional>

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
			phys_size id, phys_float numOfPhotons, phys_float n_e,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter);

		// Inherited via PAGNSimulationTeam
		virtual void run(std::string_view pathToStorageFolder) override;

	private:
		const PAGNClumpyStructureModelB& m_structureModel;
		std::optional<phys_size> m_cloudIndex;
		PSphere m_cloud;

	protected:
		// move the photon inside the agn internal structure and get the distance to
		// closest external boundary
		virtual std::optional<phys_float> distanceToBoundary(PSimplePhoton& photon)override;
	};
}// namespace agn