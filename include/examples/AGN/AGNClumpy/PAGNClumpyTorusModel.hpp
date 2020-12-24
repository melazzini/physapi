#pragma once

#include"PAGNClumpyStructureModelB.hpp"

namespace agn
{
	/**
	 * @brief This class represents a smooth sphere agn structure model.
	 *
	 * @ingroup agnsmooth
	 */
	class PAGNClumpyTorusModel :public PAGNClumpyStructureModelB
	{
		// geometry
		PSimpleTorus m_torus;

		// average column density
		phys_float m_N_H;

		std::vector<PPosition> m_clouds;

	public:
		/**
		 * Constructs a smooth sphere agn structure model.
		 *
		 * \param sphere geomtry
		 * \param N_H average column density
		 */
		PAGNClumpyTorusModel(const PSimpleTorus& torus, phys_float N_H);



		/**
		 * Get the distance to the next boundary inside the agn.
		 *
		 * \param position reference point
		 * \param direction direction of the trajectory
		 * \return optional distance
		 */
		//virtual std::optional<phys_float> distanceToBoundary(const PPosition& position, const PVector3D& direction) const override;

		/**
		 * Get the distance to the next entering point to the agn.
		 *
		 * \param position reference point
		 * \param direction direction of the trajectory
		 * \return optional distance to the  next entering point
		 */
		//virtual std::optional<phys_float> distanceToEnterinPoint(const PPosition& position, const PVector3D& direction) const override;

		/**
		 * Get the equatorial half effective size.
		 *
		 * \return the equatorial half effective size
		 */
		virtual phys_float equatorialEffectiveHalfSize() const override;

		/**
		 * Check if the given point is inside the agn structure.
		 *
		 * \param position point's position
		 * \return true if the point is inside, else false
		 */
		virtual phys_bool isInside(const PPosition& position) const override;

		/**
		 * Get the concentration of Hydrogen.
		 *
		 * \return the concentration of Hidrogen
		 */
		virtual phys_float n_H() const override;


		// Inherited via PAGNClumpyStructureModelB
		virtual const std::vector<PPosition>& clouds() const override;

	};
}// namespace agn