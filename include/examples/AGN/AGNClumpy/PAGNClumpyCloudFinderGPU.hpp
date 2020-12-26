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
	class PAGNClumpyCloudFinderGPU :public PAGNClumpyCloudFinderB
	{
	public:
		PAGNClumpyCloudFinderGPU(const std::vector<PPosition>& cloudsPositions,
			phys_float cloudsRadius);

		virtual	std::optional<phys_size> operator()(const PSimplePhoton& photon) override;

	private:
		const std::vector<PPosition>& m_cloudsPositions;

		phys_float m_cloudsRadius;

	private:
		void initializeGPU(const sPosition* clouds_h);

		void searchForCloudsGPU(const PSimplePhoton& photon);
	};
}// namespace agn