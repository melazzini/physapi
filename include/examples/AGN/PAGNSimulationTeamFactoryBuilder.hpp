#pragma once

#include"PAGNSmoothSimulationTeamFactory.hpp"

namespace agn
{

	class PAGNSimulationTeamFactoryBuilder
	{
	public:
		static inline std::shared_ptr<PAGNSimulationTeamFactory> buildAGNSmoothSimulationTeamFactory()
		{
			return std::make_shared<PAGNSmoothSimulationTeamFactory>();
		}
	};

}// namespace agn