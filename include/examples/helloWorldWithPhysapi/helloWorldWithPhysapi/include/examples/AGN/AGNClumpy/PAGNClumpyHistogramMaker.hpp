#pragma once

#include "agn_utils.hpp"

namespace agn
{

	class PAGNClumpyHistogramMaker
	{
		//this is the angular interval on theta(spherical coords)
		static constexpr phys_float ANGULAR_INTERVAL_THETA = 0.1_deg;

		//full angular interval
		static constexpr phys_float FULL_THETA_ANGULAR_INTERVAL = 359.0_deg;

		class OrientationSelectorr
		{
		public:
			OrientationSelectorr(phys_float zenithAngle, phys_float angularInterval = 10.0_deg)
				:m_zenithAngle{ zenithAngle }, m_angularInterval{ angularInterval }
			{}

			phys_bool operator()(const PVector3D& v) const
			{
				phys_float vectorZenithAngleAbsoluteValue{ Pi / 2 - std::abs(v.phi()) };

				return checkRangeInclusive(m_zenithAngle, vectorZenithAngleAbsoluteValue, m_zenithAngle + m_angularInterval);
			}

		private:
			phys_float m_zenithAngle;
			phys_float m_angularInterval;
		};

	public:
		PAGNClumpyHistogramMaker(const std::vector<PPosition>& clouds, phys_float R) :m_clouds{ clouds }, m_R{ R }{}

		t_histogram getHistogram(phys_float zenithAngle, phys_float angularInterval = 10.0_deg);

	private:
		//array of clouds(coords of their center)
		const std::vector<PPosition> & m_clouds;

		//radius of the clouds
		const phys_float m_R;

		PRandomDirection m_randDirMng;

	private:
		void countClouds(const PVector3D& dir, phys_size& numberOfCloudsOnTheCurrentInterval)const;
	};

} // end of namespace agn