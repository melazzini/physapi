#pragma once
#include "agn_utils.hpp"

namespace agn
{

	class PAGNClumpyCloudFinderB
	{
	public:

		virtual std::optional<phys_size> operator()(const PSimplePhoton& photon)=0;

	protected:
		std::optional<phys_size> mainAlgorithm(const PSimplePhoton& photon,
			const std::vector<PPosition>& cloudsPositions,
			phys_float cloudsRadius,
			phys_size firstIndex = 0// this parameter helps us to work with the gpu-cpu
		);
	};

}// namespace agn