#pragma once

#include"PAGNClumpyStructureModelB.hpp"

namespace agn
{

	class PAGNClumpyCloudsCreator
	{
		phys_float m_cloudsRadius;

		phys_size m_numOfClouds;

	public:
		PAGNClumpyCloudsCreator(phys_float cloudsRadius, phys_size numOfClouds);

		std::vector<PPosition> createClouds(PAGNClumpyStructureModelB& clumpyStructureModel);

	private:
		phys_bool noIntersection(const std::vector<phys_float>& d) const;
	};

}// namespace agn