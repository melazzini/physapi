#pragma once
#include "agn_utils.hpp"

namespace agn
{

	class PAGNClumpyCloudFinderB
	{
	public:

		virtual phys_int operator()(const PSimplePhoton& photon)=0;

	protected:
		phys_int mainAlgorithm(const PSimplePhoton& photon,
			const std::vector<PPosition>& cloudsPositions,
			phys_float cloudsRadius,
			phys_size firstIndex = 0// this parameter helps us to work with the gpu-cpu
		);
	};

}// namespace agn