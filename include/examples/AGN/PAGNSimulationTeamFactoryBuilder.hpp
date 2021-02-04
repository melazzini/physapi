#pragma once

#include"PAGNSmoothSimulationTeamFactory.hpp"
#include"PAGNClumpySimulationTeamFactory.hpp"

namespace agn
{
	/**
	 * @brief This class defines different static functions to create simulation builders.
	 * 
	 * @ingroup agn
	 */
	class PAGNSimulationTeamFactoryBuilder
	{
	public:
		static inline std::shared_ptr<PAGNSimulationTeamFactory> buildAGNSmoothSimulationTeamFactory()
		{
			return std::make_shared<PAGNSmoothSimulationTeamFactory>();
		}

		static inline std::shared_ptr<PAGNSimulationTeamFactory> buildAGNClumpySimulationTeamFactory()
		{
			return std::make_shared<PAGNClumpySimulationTeamFactory>();
		}
	};

}// namespace agn