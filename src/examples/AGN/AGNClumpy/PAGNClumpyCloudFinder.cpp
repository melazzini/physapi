#include "PAGNClumpyCloudFinder.hpp"

namespace agn
{
	std::optional<phys_size> agn::PAGNClumpyCloudFinder::operator()(const PSimplePhoton& photon,
		const std::vector<PPosition>& cloudsPositions,
		phys_float cloudsRadius)
	{
		return mainAlgorithm(photon, cloudsPositions, cloudsRadius);
	}

	std::optional<phys_size> agn::PAGNClumpyCloudFinder::mainAlgorithm(const PSimplePhoton& photon,
		const std::vector<PPosition>& cloudsPositions,
		phys_float cloudsRadius)
	{
		std::optional<phys_size> index{ std::nullopt };

		//this is the vector from the photon to the cloud
		PVector3D r_photonToCloud{};

		//this is the angle between
		//the photon direction(Omega) and
		//r_photonToCloud
		phys_float Theta{};

		//distance to the photon trajectory
		phys_float d{};

		for (phys_size i = 0; i < cloudsPositions.size(); ++i)
		{
			//initialy we get the vector from the photon
			//to the cloud: r_cloud - r_photon
			r_photonToCloud = cloudsPositions[i] - photon.position();

			//we also need to know the angle between
			//Omega & r_photonToCloud
			Theta = PVector3D::angleBetweenVectors(photon.omega(), r_photonToCloud);

			//if the cloud is "behind" the photon we discard it
			if (Theta >= (Pi / 2.0))
			{
				continue;
			}

			//the distance to the trajectory is the hypotenuse
			d = r_photonToCloud.norm() * std::sin(Theta);

			//if the distance from the cloud to the trajectory
			//is not less than its radius we discard this cloud
			if (d >= cloudsRadius)
			{
				continue;
			}

			//if this is the first cloud that could be
			//intersected by the photon then we set
			//its index, and that cloud will be a
			//"candidate"
			if (index == std::nullopt)
			{
				index = i;
				continue;
			}

			//if we already have a previous candidate
			//then we compare which cloud is closer
			//to the photon, if that previous one
			//or the current one
			if ((cloudsPositions[index.value()] - photon.position()).norm() >
				r_photonToCloud.norm())
			{
				//only if the current cloud is closer
				//we update the index of the best candidate
				index = i;
			}
		}
		return index;
	}

}//namespace agn


