#pragma once

#include"agn_utils.hpp"
#include <string_view>
namespace agn
{
	class PAGNSimulationTeam
	{
	public:
		virtual void run(std::string_view pathToStorageFolder) = 0;
	};
}// namespace agn