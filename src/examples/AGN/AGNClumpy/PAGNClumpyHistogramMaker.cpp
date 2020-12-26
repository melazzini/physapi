#include"PAGNClumpyHistogramMaker.hpp"

namespace agn
{
	t_histogram PAGNClumpyHistogramMaker::getHistogram(phys_float zenithAngle, phys_float angularInterval)
	{
		OrientationSelectorr orientationSelector{ zenithAngle, angularInterval };

		PVector3D direction_i{ 1,0,0 };
		phys_size numberOfCloudsOnTheCurrentInterval{};
		phys_size numberOfCloudsOnTheCurrentInterval_UP{};
		phys_size numberOfCloudsOnTheCurrentInterval_DOWN{};
		std::vector<phys_size> numberOfCloudsPerInterval;
		std::vector<phys_size> numberOfCloudsPerInterval_UP;
		std::vector<phys_size> numberOfCloudsPerInterval_DOWN;

		for (size_t i = 0; i < 500'000; i++)
		{
			if (i % 1'000 == 0)
			{
				std::cout << "Percentage of clouds processed(AGNHistogramMaker):    "
					<< 100.0 * (i / 500'000.0) << std::endl;
			}

			direction_i = m_randDirMng(orientationSelector);
			countClouds(direction_i, numberOfCloudsOnTheCurrentInterval);
			numberOfCloudsPerInterval.push_back(numberOfCloudsOnTheCurrentInterval);
			numberOfCloudsOnTheCurrentInterval = 0;
		}

		t_histogram histogram;

		for (auto&& numOfClouds_i : numberOfCloudsPerInterval)
		{
			histogram[numOfClouds_i]++;
		}

		phys_float totalNumberOfDirections{};

		for (auto&& item : histogram)
		{
			totalNumberOfDirections += item.second;
		}

		for (auto& item : histogram)
		{
			item.second = item.second / totalNumberOfDirections;
		}

		return histogram;
	}

	void PAGNClumpyHistogramMaker::countClouds(const PVector3D& dir, phys_size& numberOfCloudsOnTheCurrentInterval) const
	{
		for (auto&& cloud_j : m_clouds)
		{
			auto alpha = PVector3D::angleBetweenVectors(dir, cloud_j);

			if (alpha >= Pi / 2)
			{
				continue;
			}

			if (cloud_j.norm() * std::sin(alpha) < m_R)
			{
				numberOfCloudsOnTheCurrentInterval++;
			}
		}
	}
}// namespace agn