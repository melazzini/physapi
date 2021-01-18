#include"PAGN.hpp"
#include<thread>

namespace agn
{
	PAGN::PAGN(const std::shared_ptr<PAGNStructureModelB> structureModel,
		phys_float T_e, phys_float numOfPhotos,
		const std::shared_ptr<PAGNSimulationTeamFactory> simulationTeamFactory, 
		const std::shared_ptr<PAGNFormula> agnformula,
		const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter)
		:m_structureModel{ structureModel }, m_T_e{ T_e }, m_numOfPhotons{ numOfPhotos },
		m_simulationTeamFactory{ simulationTeamFactory }, m_formula{agnformula},
		m_initSpectrumDirFilter{initSpectrumDirFilter}, m_n_e{}
	{
		initTables();
		m_n_e = calculate_n_e(m_structureModel->n_H());
	}
	void PAGN::run(std::string_view pathToStorageFolder, eNumOfThreads numOfThreads)
	{
		phys_size trueNumOfThreads{1};
		if (numOfThreads == eNumOfThreads::MAX)
		{
			trueNumOfThreads = std::thread::hardware_concurrency();
		}
		else
		{
			trueNumOfThreads = static_cast<phys_size>(numOfThreads);
		}

		phys_float photonsPerThread = m_numOfPhotons / static_cast<phys_float>(trueNumOfThreads);

		std::vector<std::thread> threads;

		for (phys_size i = MAINTHREAD_ID + 1; i < trueNumOfThreads; i++)
		{
			threads.push_back(std::thread(&PAGN::createSimulationThread, this, i, photonsPerThread, pathToStorageFolder));
		}

		createSimulationThread(MAINTHREAD_ID, photonsPerThread, pathToStorageFolder);

		for (auto& thread_i : threads)
		{
			if (thread_i.joinable())
			{
				thread_i.join();
			}
		}
	}
	void PAGN::initTables()
	{
		m_abundances = std::make_shared<PAbundanceTable>();
		initVernerTable1();
		initVernerTable2();
		m_fluorescences = std::make_shared<PFluorescenceTable>(*m_abundances);
	}

	void PAGN::initVernerTable1()
	{
		// this configurator object handles the verner table1 proper configuration.
		// the order of elements in that table is not well suited for our algorithms
		// that is why we need to tacle that issue and set an order with the quantum numbers.
		// this object handle that issue
		PVernerTable1Hnd config;

		// ProcessTable can load data with special configuration requirements, in this case
		// the requirement is that the elements in verner table 1 will be properly ordered
		// when they get loaded into the table object. Thus, handler1 will handle that issue
		PProcessTable<t_vernertable1Key, t_vernertable1Value, VERNERTABLE1_COLS, PLoadVernerTable1> handler1{ "./include/astrophysics/tables/vernertable1.txt", config };

		// This selector will filter out the elements that aren't neutral
		PSelectorVernerTable1 f;

		m_vernerTable1 = std::make_shared<PVernerTable1>(*m_abundances);

		// now we can load up(in a special way) the verner table 1 object 
		handler1(*m_vernerTable1, f, *m_abundances);
	}
	void PAGN::initVernerTable2()
	{
		m_vernerTable2 = std::make_shared<PVernerTable2>(*m_abundances);

		// verner table 2 does not has any special requirements, other than the existence
		// of the elements, that is determined by the abundace table, this is why we use
		// the loadFromFile() function directly to load the elements of verner table 2.
		loadFromFile<VERNERTABLE2_COLS>(DEFAULT_FILE_PATH_VERNERTABLE2, *m_vernerTable2);
	}
	void PAGN::createSimulationThread(phys_size id, phys_float numOfPhotons, std::string_view pathToStorageFolder)
	{
		auto simulationTeam{ m_simulationTeamFactory->buildSimulationTeam(m_structureModel,
																		m_vernerTable1,m_vernerTable2,
																		m_fluorescences, m_abundances,
																		id, numOfPhotons, m_n_e, m_T_e,
																		m_formula, m_initSpectrumDirFilter) };
		simulationTeam->run(pathToStorageFolder);
	}
}// namespace agn