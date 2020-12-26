#include"PAGNClumpyCloudFinderGPU.hpp"

namespace agn
{
	PAGNClumpyCloudFinderGPU::PAGNClumpyCloudFinderGPU(const std::vector<PPosition>& cloudsPositions,
		phys_float cloudsRadius)
		:m_cloudsPositions{ cloudsPositions }, m_cloudsRadius{ cloudsRadius }
	{
	}
	void PAGNClumpyCloudFinderGPU::initializeGPU(const sPosition* clouds_h)
	{
	}
	void PAGNClumpyCloudFinderGPU::searchForCloudsGPU(const PSimplePhoton& photon)
	{
	}
}// namespace agn