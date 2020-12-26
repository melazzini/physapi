#include "PAGNClumpyCloudFinder.hpp"

namespace agn
{
	PAGNClumpyCloudFinder::PAGNClumpyCloudFinder(
		const std::vector<PPosition>& cloudsPositions,
		phys_float cloudsRadius)
		:m_cloudsPositions{ cloudsPositions }, m_cloudsRadius{ cloudsRadius }
	{
	}

	std::optional<phys_size> agn::PAGNClumpyCloudFinder::operator()(const PSimplePhoton& photon)
	{
		return mainAlgorithm(photon, m_cloudsPositions, m_cloudsRadius);
	}

}//namespace agn


