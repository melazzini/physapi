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

		// average number of clouds along the equator
		phys_float m_averNumCloudsEquator;

		// volume filling factor
		phys_float m_volFillFactor;

		std::vector<PPosition> m_clouds;

		phys_float m_cloudsRadius;

		PRandom m_rndMng;

		// concentration of hydrogen
		phys_float m_n_H;

		// average column density of the clouds
		phys_float m_N_H_clouds;

		// total number of clouds
		phys_size m_N_clouds_tot;

	public:

		/**
		 * Constructs a smooth sphere agn structure model.
		 * 
		 * \param torus
		 * \param N_H average column density along the equator
		 * \param averNumOfCloudsEquator average number of clouds along the equator
		 * \param volFillFactor volume filling factor
		 */
		PAGNClumpyTorusModel(const PSimpleTorus& torus,
			phys_float N_H,
			phys_float averNumOfCloudsEquator,
			phys_float volFillFactor);

		PAGNClumpyTorusModel(const PSimpleTorus& torus,
			phys_float N_H,
			phys_float averNumOfCloudsEquator,
			phys_float volFillFactor,
			const t_clouds& clouds);

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
		virtual phys_float n_H() const override { return m_n_H; }


		// Inherited via PAGNClumpyStructureModelB
		virtual const std::vector<PPosition>& clouds() const override;

		virtual phys_float cloudRadius()const override { return m_cloudsRadius; }

		/**
		 * Place a cloud inside the agn randomly.
		 *
		 * \param cloudPosition
		 */
		virtual void placeCloud(PPosition& cloudPosition) override;

	private:

		phys_float calculateN_H_clouds() const
		{
			return m_N_H / m_averNumCloudsEquator;
		}

		phys_float calculate_n_H() const
		{
			return m_N_H / (m_volFillFactor * (m_torus.externalRadius() - m_torus.internalRadius()));
		}

		phys_float calculateCloudsRadius() const
		{
			return (3. / 4.) * (m_N_H / (m_n_H * m_averNumCloudsEquator));
		}

		phys_float calculateNumOfClouds() const
		{
			return (3. / 4.) * (m_volFillFactor / Pi) * (m_torus.volume() / cub(m_cloudsRadius));
		}
	};
}// namespace agn