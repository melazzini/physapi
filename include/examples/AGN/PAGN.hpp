#pragma once

#include"PAGNSmoothStructureModelB.hpp"
#include<string_view>
#include<memory>
#include"verner_utils.hpp"
#include"PAGNStructureModelB.hpp"
#include"PAGNSimulationTeamFactory.hpp"

namespace agn
{
	/**
	 * This class represents a smooth and homogeneous AGN.
	 *
	 * @ingroup agnsmooth
	 */
	class PAGN
	{
		// ID of the main thread
		static constexpr phys_size MAINTHREAD_ID = 0U;

	public:

		/**
		 * Constructs a smooth agn x-ray spectral model.
		 *
		 * \param structureModel agn structure model
		 * \param T_e temperature of the electrons
		 * \param numOfPhotos number of photons
		 */
		PAGN(const std::shared_ptr<PAGNStructureModelB> structureModel,
			phys_float T_e,
			phys_float numOfPhotos,
			const std::shared_ptr<PAGNSimulationTeamFactory> simulationTeamFactory,
			const std::shared_ptr<PAGNFormula> agnformula);

		/**
		 * Run the simulation of the agn x-ray spectral model.
		 *
		 * \param PathToFolder path to storage folder(the data resulting from the simulation will be stored here)
		 * \param numOfThreads number of cpu threads to be used in the simulation
		 */
		void run(std::string_view PathToFolder, eNumOfThreads numOfThreads = eNumOfThreads::MAX);

	private:

		const std::shared_ptr< PAGNStructureModelB> m_structureModel;
		phys_float m_T_e;
		phys_float m_numOfPhotons;
		const std::shared_ptr<PAGNSimulationTeamFactory> m_simulationTeamFactory;
		std::shared_ptr<PAbundanceTable> m_abundances;
		std::shared_ptr<PVernerTable1> m_vernerTable1;
		std::shared_ptr<PVernerTable2> m_vernerTable2;
		std::shared_ptr<PFluorescenceTable> m_fluorescences;
		const std::shared_ptr<PAGNFormula> m_formula;

	private:
		void initTables();
		void initVernerTable1();
		void initVernerTable2();
		void createSimulationThread(phys_size id, phys_float numOfPhotons,
			std::string_view pathToStorageFolder);
	};
}