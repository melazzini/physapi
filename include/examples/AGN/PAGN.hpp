#pragma once

#include"PAGNSmoothStructureModelB.hpp"
#include<string_view>
#include<memory>
#include"verner_utils.hpp"
#include"PAGNStructureModelB.hpp"
#include"PAGNSimulationTeamFactory.hpp"
#include"PAGNInitSpectrumDirectionFilter.hpp"

namespace agn
{
	/**
	 * @brief This class represents an AGN x-ray spectral model(agn model).
	 *
	 * This class represents a generic AGN model. It can be homogeneous or it can be clumpy.
	 * In addition, the geometric shape of the galaxy can also be specified separately.
	 * The geometry is absolutely independent of the internal structure. Furthermore,
	 * the physics and the Monte-Carlo algorithm for simulation are also independent and
	 * in overall the system is very flexible to be used for studying the physical processes
	 * involved in the formation of the resulting spectrum as well as for the dependency
	 * of the resulting spectrum on different parameters, such as the geometric shape,
	 * internal structure, number of clouds along the observer's line, etc. 
	 * 
	 * To be able to use the class:
	 * 
	 *	1. Design your structure model(geometry+internal structure) based on PAGNStructureModelB.
	 *	   For example a simple clumpy torus.
	 *  2. Create your simulation team based on PAGNSimulationTeamB.
	 *  3. Create the factory, which builds simulation team. To do this use the class PAGNSimulationTeamFactory.
	 *  4. Define your model's spectral formula based on PAGNFormula.
	 *  5. If you want to have your photons created under some specific angular interval,
	 *     then derive your filter from PAGNInitSpectrumDirectionFilter.
	 * 
	 * @note Think of your simulation team as the group(for each thread) of internal objects, that handle
	 *		  different aspects of the simulation. For example photo-absorption, tracking the photons on
	 *	     their trajectory until they get registered, etc. For more information see PAGNSimulationTeamB.
	 * 
	 * @note Note that we have already implemented an example of a simulation team with the corresponding simulation 
	 *       algorithm which can be used very easily, and you are only required to implement one protected function.
	 *	     For more information please refer to PAGNSimulationTeamB.
	 * 
	 * @note The use of a factory to instantiate your simulation team class makes the model more flexible and separates the logic, and
	 *		 this is why we've chosen this approach.
	 * 
	 * The following piece of code shows how we you can model an agn with a smooth simple toroidal structure.
	 * 
	 @code{.cpp}
		#include <iostream>
		#include<string_view>
		#include"PAGNSimulationTeamFactoryBuilder.hpp"
		#include"PAGN.hpp"
		#include"PAGNSmoothTorus.model.hpp"
		#include"PAGNSimpleTorusDirectionFilter.hpp"

		using namespace physapi;
		using namespace agn;
		using namespace std;

		constexpr phys_float E_low = 100.0;
		constexpr phys_float E_upp = 300.0E3;
		constexpr phys_float T_e = 1.0_eVToKelvin;
		constexpr phys_float TORUS_R1 = 1.0E14_cm;
		constexpr phys_float TORUS_R2 = 1.0E15_cm;
		constexpr phys_float TORUS_HALFOPENINGANGLE = 60.0_deg;
		constexpr phys_float N_H = 1.0E24 / 1.0_cm2;
		constexpr phys_float N_aver = 3;             //average number of clouds along the line of sight
		constexpr phys_float phi = 0.03;             //volume filling factor
		constexpr phys_float NUM_OF_PHOTONS = 50.0E6;
		const string PATH_TO_FOLDER = "./results/tests/24/";
		const string PATH_TO_FOLDER_DATA = PATH_TO_FOLDER + "data/";

		int main()
		{

			auto torusModel{ make_shared<PAGNSmoothTorusModel>(PSimpleTorus(TORUS_R1,TORUS_R2,TORUS_HALFOPENINGANGLE),N_H) };
			auto formula{ make_shared<PAGNFormula>(E_low,E_upp) };
			auto torusDirFilter{ make_shared<PAGNSimpleTorusDirectionFilter>(TORUS_HALFOPENINGANGLE) };

			auto agn{ PAGN{torusModel,T_e,NUM_OF_PHOTONS,PAGNSimulationTeamFactoryBuilder::buildAGNSmoothSimulationTeamFactory(),formula,torusDirFilter} };

			agn.run(PATH_TO_FOLDER_DATA); // use maximum number of threads

			return 0;
		}
		@endcode
	 * 
	 * @ingroup agn
	 */
	class PAGN
	{
		// ID of the main thread
		static constexpr phys_size MAINTHREAD_ID = 0U;

	public:

		/**
		 * @brief Constructs an agn x-ray spectral model.
		 * 
		 * \param structureModel geometry + internal structure
		 * \param T_e temperature of the electrons
		 * \param numOfPhotos number of photons in the simulation
		 * \param simulationTeamFactory creates the group of objects needed for the simulation
		 * \param agnformula spectral formula of the agn
		 * \param initSpectrumDirFilter determines the initial direction of the photons
		 * 
		 * @note By default all initial directions are allowed.
		 */
		PAGN(const std::shared_ptr<PAGNStructureModelB> structureModel,
			phys_float T_e,
			phys_float numOfPhotos,
			const std::shared_ptr<PAGNSimulationTeamFactory> simulationTeamFactory,
			const std::shared_ptr<PAGNFormula> agnformula,
			const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter = std::make_shared<PAGNInitSpectrumDirectionFilter>());

		/**
		 * @brief Run the simulation of the agn x-ray spectral model.
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
		const std::shared_ptr<PAGNInitSpectrumDirectionFilter> m_initSpectrumDirFilter;
		phys_float m_n_e; // concentration of electrons
	private:
		void initTables();
		void initVernerTable1();
		void initVernerTable2();
		void createSimulationThread(phys_size id, phys_float numOfPhotons,
			std::string_view pathToStorageFolder);
		// calculate concentration of electrons
		phys_float calculate_n_e(phys_float n_H) const 
		{
			return m_abundances->total() * n_H;
		}
	};
}