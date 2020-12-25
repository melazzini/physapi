#include"PAGNClumpyCloudsCreator.hpp"

namespace agn
{
	PAGNClumpyCloudsCreator::PAGNClumpyCloudsCreator(phys_float cloudsRadius, phys_size numOfClouds)
		:m_cloudsRadius{cloudsRadius}, m_numOfClouds{numOfClouds}
	{
	}
	std::vector<PPosition> PAGNClumpyCloudsCreator::createClouds(PAGNClumpyStructureModelB& clumpyStructureModel)
	{
		std::vector<PPosition> clouds;
		
		clouds.reserve(m_numOfClouds);

		PPosition cloud_i_pos;

		// container of distances between 
        // already placed clouds and 
        // the one we are trying to place
		std::vector<phys_float> d;

        for (phys_size i = 0; i < m_numOfClouds; i++)
        {
            if (i % phys_size(0.01 * m_numOfClouds) == 0)
            {
                std::cout << "Clouds created: "
                    << 100.0 * phys_float(i) / m_numOfClouds
                    << "%       \r";
                std::cout << std::flush;
            }

            do
            {
                clumpyStructureModel.placeCloud(cloud_i_pos);

                d.clear();

                for (phys_size j = 0; j < i; j++)
                {
                    d.push_back((cloud_i_pos - clouds[j]).norm());
                }

            } while (!noIntersection(d));

            clouds.push_back(cloud_i_pos);

            // reset
            cloud_i_pos = PPosition();
        }

		return std::move(clouds);
	}
    phys_bool PAGNClumpyCloudsCreator::noIntersection(const std::vector<phys_float>& d) const
    {
        if (d.size() == 0)
        {
            return true;
        }

        for (phys_size i = 0; i < d.size(); i++)
        {
            if (d[i] < (2 * m_cloudsRadius))
            {
                return false;
            }
        }

        return true;
    }
}// namespace agn