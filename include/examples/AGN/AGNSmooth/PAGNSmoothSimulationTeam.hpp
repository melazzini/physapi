#pragma once
#include"PAGNSimulationTeam.hpp"
#include"PAGNSmoothStructureModelB.hpp"
#include"PAGNSimulationMng.hpp"
#include"PAGNFormula.hpp"
namespace agn
{
	class PAGNSmoothSimulationTeam : public PAGNSimulationTeam
	{
	private:
		static constexpr phys_size N_intervals = 2000;
		static constexpr phys_size SPECTRUM_PRECISION = 10'000'000;

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
		const PAGNStructureModelB& m_structureModel;
		const std::shared_ptr<PVernerTable1> m_vernerTable1;
		const std::shared_ptr<PVernerTable2> m_vernerTable2;
		const std::shared_ptr<PFluorescenceTable> m_fluorescenceTable;
		const std::shared_ptr<PAbundanceTable> m_abundances;
		phys_size m_id;
		phys_float m_numOfPhotons;
		PVerner m_verner;
		PFluorescenceHnd m_fluorescenceHnd;
		PAGNSimulationMng m_simMng;
		const std::shared_ptr<PAGNFormula> m_formula;
		PSpectrumMaker<N_intervals, SPECTRUM_PRECISION> m_spectrumMaker;
		PSpectrum<N_intervals> m_initSpectrum;
	};
}// namespace agn