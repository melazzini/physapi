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
		 * \brief Get the distance to the next entering point to the agn.
		 *
		 * \param position reference point
		 * \param direction direction of the trajectory
		 * \return optional distance to the entering point
		 */
		virtual std::optional<phys_float> distanceToEnteringPoint(const PPosition& position, const PVector3D& direction)const = 0;
	};
}// namespace agn
