#pragma once
#include "PAGNClumpyCloudFinderB.hpp"

namespace agn
{
	//This class finds the next cloud,
	//which is intersected by a
	//photon on its trajectory, if
	//there exists such a cloud.
	//Dont use this class if the current
	//photon is inside a cloud already!
	class PAGNClumpyCloudFinder :public PAGNClumpyCloudFinderB
	{
	public:

		PAGNClumpyCloudFinder(const std::vector<PPosition>& cloudsPositions,
			phys_float cloudsRadius);

		phys_int operator()(const PSimplePhoton& photon) override;

	private:
		const std::vector<PPosition>& m_cloudsPositions;

		phys_float m_cloudsRadius;
	};
}// namespace agn