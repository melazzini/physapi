#pragma once
#include"agn_utils.hpp"

namespace agn
{
	class PAGNStructureModelB
	{
	public:
		/**
		 * Get the equatorial effective half size of the simple agn structure model.
		 *
		 * \return the equatorial effective half size
		 */
		virtual phys_float equatorialEffectiveHalfSize()const = 0;

		/**
		 * Check whether the given point is inside the agn.
		 *
		 * \param position position of the point
		 * \return true if the point is inside the agn, else false
		 */
		virtual phys_bool isInside(const PPosition& position)const = 0;

		/**
		 * Get the concentration of hydrogen in the agn.
		 *
		 * \return the concentration of hydrogen
		 */
		virtual phys_float n_H() const = 0;
	};
}