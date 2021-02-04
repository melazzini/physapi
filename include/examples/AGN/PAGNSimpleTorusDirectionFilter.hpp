#pragma once

#include"PAGNInitSpectrumDirectionFilter.hpp"

namespace agn
{
	/**
	 * @brief This class selects the initial directions for the source photons for the simple torus agn model.
	 * 
	 * @ingroup agn
	 */
	class PAGNSimpleTorusDirectionFilter :public PAGNInitSpectrumDirectionFilter
	{
	private:
		phys_float m_halfOpeningAngle;
	public:

		/**
		 * @brief Creates the direction filter object.
		 * 
		 * \param halfOpeningAngle torus half-opening angle
		 */
		PAGNSimpleTorusDirectionFilter(phys_float halfOpeningAngle)
			:m_halfOpeningAngle{halfOpeningAngle}
		{
			
		}

		/**
		 * @brief Check whether or not the given direction is valid.
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