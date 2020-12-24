#include"PAGNSimulationTeamB.hpp"

namespace agn
{
	PAGNSimulationTeamB::PAGNSimulationTeamB(
		const std::shared_ptr<PVernerTable1> vernerTable1, 
		const std::shared_ptr<PVernerTable2> vernerTable2, 
		const std::shared_ptr<PFluorescenceTable> fluorescenceTable, 
		const std::shared_ptr<PAbundanceTable> abundances, 
		phys_size id, phys_float numOfPhotons, 
		const std::shared_ptr<PAGNFormula> agnformula)
		: m_vernerTable1{vernerTable1}, m_vernerTable2{vernerTable2},
		m_fluorescenceTable{fluorescenceTable}, m_abundances{abundances},
		m_id{ id }, m_numOfPhotons{ numOfPhotons }, 
		m_formula{ agnformula },
		m_verner{*m_abundances, *m_vernerTable1, *m_vernerTable2},
		m_fluorescenceHnd{m_fluorescenceTable},m_simMng{}, 
		m_initSpectrum(m_formula->E_low(),m_formula->E_cut(),eSpectrumScale::LOG)
	{
		PSpectrumMaker<N_intervals, SPECTRUM_PRECISION> spectrumMaker;

		// initialize the intrinsic spectrum spectrum
		spectrumMaker(m_initSpectrum, numOfPhotons, *m_formula);

		if (m_id == 0)
		{

			std::cout << "Simulation team created(main thread)!" << std::endl;
			std::cout << "number of photons per thread (main thread):  " << m_initSpectrum.count() << std::endl;
		}
		else
		{
			std::cout << "New thread created, with id:	" << m_id << std::endl;
		}
	}

}// namespace agn