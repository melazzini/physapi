#pragma once

#include"agn_utils.hpp"

namespace agn
{
	/**
	 * @brief This functor filters the initial directions of the photons
	 * generated in the simulation.
	 *
	 * @ingroup agn
	 *
	 */
	class PAGNInitSpectrumDirectionFilter
	{

	public:

		/**
		 * Check whether or not the given direction is valid.
		 * 
		 * \param direction photon's direction
		 * \return true if the direction is valid, else false
		 */
		virtual phys_bool operator()(const PVector3D& direction) const
		{
			return true;
		}
	};
}// namespace agn