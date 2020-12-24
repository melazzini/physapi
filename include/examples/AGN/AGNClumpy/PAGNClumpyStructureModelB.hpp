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
	class PAGNClumpyStructureModelB :public PAGNStructureModelB
	{
	public:
		
		/**
		 * Get a reference to the clouds positions.
		 *
		 * \return clouds positions
		 */
		virtual const std::vector<PPosition>& clouds()const = 0;
	};
}// namespace agn
