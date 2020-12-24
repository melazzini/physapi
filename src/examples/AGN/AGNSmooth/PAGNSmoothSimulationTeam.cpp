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
		:m_structureModel{ static_cast<const PAGNSmoothStructureModelB&>(*structureModel) },
		m_vernerTable1{ vernerTable1 }, m_vernerTable2{ vernerTable2 },
		m_fluorescenceTable{ fluorescenceTable }, m_abundances{ abundances },
		m_id{ id }, m_numOfPhotons{ numOfPhotons },
		m_verner{ *m_abundances,*m_vernerTable1, *m_vernerTable2 },
		m_fluorescenceHnd{ fluorescenceTable }, m_simMng{},
		m_formula{ agnformula }, m_spectrumMaker{},
		m_initSpectrum{ agnformula->E_low(),agnformula->E_cut(), eSpectrumScale::LOG }
	{
		// initialize the intrinsic spectrum spectrum
		m_spectrumMaker(m_initSpectrum, numOfPhotons, *m_formula);

		if (m_id == 0)
		{
			std::cout << "number of photons main thread:  " << m_initSpectrum.count() << std::endl;
		}
	}
	void PAGNSmoothSimulationTeam::run(std::string_view pathToStorageFolder)
	{

	}
}// namespace agn