#include"PAGNClumpyTorusModel.hpp"
#include"PAGNClumpyCloudsCreator.hpp"
namespace agn
{
	PAGNClumpyTorusModel::PAGNClumpyTorusModel(
		const PSimpleTorus& torus, 
		phys_float N_H,
		phys_float averNumOfCloudsEquator,
		phys_float volFillFactor)
		:m_torus{ torus }, m_N_H{ N_H },
		m_averNumCloudsEquator{averNumOfCloudsEquator},
		m_volFillFactor{volFillFactor},
		m_clouds{}, m_cloudsRadius{}, m_rndMng{}
	{
		// ! the order here is very important

		m_N_H_clouds = calculateN_H_clouds();
		m_n_H = calculate_n_H();
		m_cloudsRadius = calculateCloudsRadius();
		m_N_clouds_tot = calculateNumOfClouds();
		std::cout << "Total number of clouds:  " << m_N_clouds_tot << std::endl;
		std::cout << "Creating clouds!" << std::endl;
		std::cout << std::endl;
		
		PAGNClumpyCloudsCreator cloudsCreator{ m_cloudsRadius, m_N_clouds_tot };
		m_clouds.reserve(m_N_clouds_tot);
		m_clouds = cloudsCreator.createClouds(*this);

		std::cout << "Creation of clouds done!" << std::endl;
		std::cout << std::endl;		
	}
	PAGNClumpyTorusModel::PAGNClumpyTorusModel(const PSimpleTorus& torus, 
		phys_float N_H, 
		phys_float averNumOfCloudsEquator, 
		phys_float volFillFactor, 
		const t_clouds& clouds)
		:m_torus{ torus }, m_N_H{ N_H },
		m_averNumCloudsEquator{ averNumOfCloudsEquator },
		m_volFillFactor{ volFillFactor },
		m_clouds{clouds}, m_cloudsRadius{}, m_rndMng{}
	{
		// ! the order here is very important

		m_N_H_clouds = calculateN_H_clouds();
		m_n_H = calculate_n_H();
		m_cloudsRadius = calculateCloudsRadius();
		m_N_clouds_tot = calculateNumOfClouds();
		std::cout << "Total number of clouds:  " << m_N_clouds_tot << std::endl;

		if (m_N_clouds_tot != m_clouds.size())
		{
			std::cerr << "PAGNClumpyTorusModel error while loading the clouds: "
				<< "Wrong number of clouds!" << std::endl;
			abort();
		}

		std::cout << std::endl;
	}
	phys_float PAGNClumpyTorusModel::equatorialEffectiveHalfSize() const
	{
		return m_torus.externalRadius() - m_torus.internalRadius();
	}
	phys_bool PAGNClumpyTorusModel::isInside(const PPosition& position) const
	{
		return m_torus.isInside(position);
	}

	const std::vector<PPosition>& PAGNClumpyTorusModel::clouds() const
	{
		return m_clouds;
	}
	void PAGNClumpyTorusModel::placeCloud(PPosition& cloudPosition)
	{
		do
		{
			// we place each cloud inside a cub:
			cloudPosition.setX(m_rndMng.uniform_real(-(m_torus.externalRadius() - m_cloudsRadius),
				(m_torus.externalRadius() - m_cloudsRadius))); // length

			cloudPosition.setY(m_rndMng.uniform_real(-(m_torus.externalRadius() - m_cloudsRadius),
				(m_torus.externalRadius() - m_cloudsRadius))); // width

			cloudPosition.setZ(m_rndMng.uniform_real(-(m_torus.externalRadius() - m_cloudsRadius),
				(m_torus.externalRadius() - m_cloudsRadius))); // height

			phys_float phi_h{ std::abs(cloudPosition.phi()) };
			phys_float max_phi_h{ Pi / 2 - (2 * std::asin(m_cloudsRadius / (2 * cloudPosition.norm())) + m_torus.theta0()) };

		} while (!(std::abs(cloudPosition.phi()) < Pi / 2 - (2 * std::asin(m_cloudsRadius / (2 * cloudPosition.norm())) + m_torus.theta0())) ||
			!checkRangeInclusive(m_torus.internalRadius() + m_cloudsRadius, cloudPosition.norm(), m_torus.externalRadius() - m_cloudsRadius));
	}
}// namespace agn