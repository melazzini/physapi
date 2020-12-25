#pragma once
#include "agn_utils.hpp"

namespace agn
{
	//This class finds the next cloud,
	//which is intersected by a
	//photon on its trajectory, if
	//there exists such a cloud.
	//Dont use this class if the current
	//photon is inside a cloud already!
	class PAGNClumpyCloudFinder
	{
	public:

		std::optional<phys_size> operator()(const PSimplePhoton& photon, 
			const std::vector<PPosition>& cloudsPositions, 
			phys_float cloudsRadius);

	private:
		std::optional<phys_size> mainAlgorithm(const PSimplePhoton& photon,
			const std::vector<PPosition>& cloudsPositions,
			phys_float cloudsRadius);
	};
}// namespace agn