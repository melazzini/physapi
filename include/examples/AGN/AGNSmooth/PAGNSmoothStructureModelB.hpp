/*****************************************************************//**
 * \file   PAGNSmoothStructureModelB.hpp
 * \brief  This file contains the declaration of the PSimpleAGNStrucure model
 *		   abstract base class.
 *
 * \author francisco melazzini
 * \date   December 2020
 *********************************************************************/
#pragma once
#include"agn_utils.hpp"
#include<optional>
#include "PAGNStructureModelB.hpp"

namespace agn
{
	/**
	 * @brief This base class represents an abstract simple(not tend to be clumpy)
	 *		  agn structure model, it can be spherical, toridal, etc.
	 *
	 * @ingroup agnsmooth
	 */
	class PAGNSmoothStructureModelB :public PAGNStructureModelB
	{
	public:

		/**
		 * \brief Get the distance to the next boundary inside the agn.
		 *
		 * \param position reference point
		 * \param direction direction of the trajectory
		 * \return optional distance to the next boundary
		 */
		virtual std::optional<phys_float> distanceToBoundary(const PPosition& position, const PVector3D& direction)const = 0;

		/**
		 * \brief Calculate(with probability) the distance to the interaction point.
		 *
		 * \param lamdaMean mean free path
		 * \param escapeProbability escape probability
		 * \param rand_0_1 random value in 0..1
		 * \return distance to the interaction point
		 */
		virtual phys_float distanceToInteractionPoint(
			phys_float lamdaMean,
			phys_float escapeProbability,
			phys_float rand_0_1)const
		{
			return -lamdaMean * std::log(1 - rand_0_1 * (1 - escapeProbability));
		}

		/**
		 * \brief Get the distance to the next entering point to the agn.
		 *
		 * \param position reference point
		 * \param direction direction of the trajectory
		 * \return optional distance to the entering point
		 */
		virtual std::optional<phys_float> distanceToEnterinPoint(const PPosition& position, const PVector3D& direction)const = 0;
	};
}// namespace agn
