#pragma once

#include"PAGNSmoothStructureModelB.hpp"

namespace agn
{
	/**
	 * @brief This class represents a smooth torus agn structure model.
	 *
	 * This class represents an AGN with a smooth isotropic toroidal structure with a given
	 * column density \f$ N_H \f$.
	 * 
	 * @ingroup agn
	 */
	class PAGNSmoothTorusModel :public PAGNSmoothStructureModelB
	{
		// geometry
		PSimpleTorus m_torus;

		// average column density
		phys_float m_N_H;

	public:
		/**
		 * @brief Constructs a smooth torus agn structure model.
		 *
		 * \param torus geomtry
		 * \param N_H average column density
		 */
		PAGNSmoothTorusModel(const PSimpleTorus& torus, phys_float N_H);

		/**
		 * @brief Get the distance to the next boundary inside the agn.
		 *
		 * \param position reference point
		 * \param direction direction of the trajectory
		 * \return optional distance
		 */
		virtual std::optional<phys_float> distanceToBoundary(const PPosition& position, const PVector3D& direction) const override;

		/**
		 * @brief Get the distance to the next entering point to the agn.
		 *
		 * \param position reference point
		 * \param direction direction of the trajectory
		 * \return optional distance to the  next entering point
		 */
		virtual std::optional<phys_float> distanceToEnteringPoint(const PPosition& position, const PVector3D& direction) const override;

		/**
		 * @brief Get the equatorial half effective size.
		 *
		 * \return the equatorial half effective size
		 */
		virtual phys_float equatorialEffectiveHalfSize() const override;

		/**
		 * @brief Check if the given point is inside the agn structure.
		 *
		 * \param position point's position
		 * \return true if the point is inside, else false
		 */
		virtual phys_bool isInside(const PPosition& position) const override;

		/**
		 * @brief Get the concentration of Hydrogen.
		 *
		 * \return the concentration of Hydrogen
		 */
		virtual phys_float n_H() const override;

	};

}// namespace agn