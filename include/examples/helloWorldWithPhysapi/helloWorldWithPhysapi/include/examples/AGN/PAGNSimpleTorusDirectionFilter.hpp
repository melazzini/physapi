#pragma once

#include"PAGNInitSpectrumDirectionFilter.hpp"

namespace agn
{
	class PAGNSimpleTorusDirectionFilter :public PAGNInitSpectrumDirectionFilter
	{
	private:
		phys_float m_halfOpeningAngle;
	public:

		PAGNSimpleTorusDirectionFilter(phys_float halfOpeningAngle)
			:m_halfOpeningAngle{halfOpeningAngle}
		{
			
		}

		/**
		 * Check whether or not the given direction is valid.
		 *
		 * \param direction photon's direction
		 * \return true if the direction is valid, else false
		 */
		virtual phys_bool operator()(const PVector3D& direction) const override
		{
			// this is for the simple torus
			return std::abs(direction.phi()) <= (Pi / 2 - m_halfOpeningAngle);
		}
	};
}// namespace agn