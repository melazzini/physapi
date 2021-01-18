#include"PAGNClumpyCloudFinderGPU.hpp"
#include<optional>
namespace agn
{

	PAGNClumpyCloudFinderGPU::PAGNClumpyCloudFinderGPU(const std::vector<PPosition>& cloudsPositions,
		phys_float cloudsRadius)
		:m_cloudsPositions{ cloudsPositions },
		m_cloudsRadius{ cloudsRadius },
		m_gpuThreadsOrganizer{ m_cloudsPositions.size() },
		m_indexes_host{ nullptr },
		m_indexes_device{ nullptr },
		m_cloudPos_device{ nullptr }
	{
#ifdef PHYSAPI_USE_GPU
		cudaStreamCreate(&m_cudaStream);

		// first we have to prepare the data(positions of clouds) to pass it
		// to the gpu, ie we allocate momentarily the position of the clouds
		// in the heap
		sPosition* clouds_host_here{ new sPosition[m_cloudsPositions.size()] };

		for (phys_size i = 0; i < m_cloudsPositions.size(); i++)
		{
			clouds_host_here[i].x = m_cloudsPositions[i].x();

			clouds_host_here[i].y = m_cloudsPositions[i].y();

			clouds_host_here[i].z = m_cloudsPositions[i].z();
		}

		// we initialize the memory in the gpu
		initializeGPU(clouds_host_here);

		// we now can delete the helper memory in the heap
		delete[] clouds_host_here;

		cudaMallocHost(&m_indexes_host, m_gpuThreadsOrganizer.getNumOfGPUBlocks() * sizeof(phys_size));

		for (phys_size i = 0; i < m_gpuThreadsOrganizer.getNumOfGPUBlocks(); i++)
		{
			m_indexes_host[i] = ULLONG_MAX;
		}
#endif // PHYSAPI_USE_GPU


		// at this point, we have the position of the clouds and the array of
		// possible indexes(ie clouds that are intersected by the photon trajectory)
		// both loaded in the gpu
	}
	phys_int PAGNClumpyCloudFinderGPU::operator()(const PSimplePhoton& photon)
	{
#ifdef PHYSAPI_USE_GPU
		// first we will get the best candidate among the clouds processed by the cpu
		auto firstIndexOfCloudsInTheCPU = m_cloudsPositions.size() - m_gpuThreadsOrganizer.getNumOfCPUClouds();

		auto bestCPUIndex = mainAlgorithm(photon, m_cloudsPositions, firstIndexOfCloudsInTheCPU);

		// the remaining clouds will be examine in the gpu
		searchForCloudsGPU(photon);

		// first initialize to the heighest possible values
		auto distancePhotonToBestCloud{ DBL_MAX };
		phys_size bestGPUIndex{ ULLONG_MAX };

		// and now iterate through all the elements
		auto photonPosition{ photon.position() };
		for (phys_size i = 0; i < m_gpuThreadsOrganizer.getNumOfGPUBlocks(); i++)
		{
			auto index{ m_indexes_host[i] };

			// the kernels set this index to ULLONG_MAX if the corresponding gpu block
			// did not find any cloud, in that block, on the photon's trajectory
			// thus if the index has that value we skip to the next cloud
			if (index == ULLONG_MAX)
			{
				continue;
			}

			// if we get here, ie the index "points" to a valid cloud-candidate
			// that is intersected by the photon's trajectory, thus we need to get
			// the distance from the photon to that cloud

			// get the position of the cloud
			auto cloudPosition{ m_cloudsPositions[index] };

			// we do not need to find the actual distance from the photon
			// to the point in the cloud where, the photon would enter into it,
			// because we work with spheres we only need to know the distance
			// from the photon to the center of the cloud, in other words,
			// we need "the distance from the photon to the cloud"
			auto currentDistance{ (photonPosition - cloudPosition).norm() };

			// if the current distance is less than the distance
			// calculated in the previous iteration we set this value
			// as the distance to the best current candidate, and set
			// store its index
			if (currentDistance < distancePhotonToBestCloud)
			{
				distancePhotonToBestCloud = currentDistance;
				bestGPUIndex = index;
			}
		}

		// if the variable distancePhotonToBestCloud has a meaningful value
		// then it means that the corresponding index "points" to a valid cloud
		// and therefore we can use its value, thus we initialize the next
		// variable as follows, notice that in the agn-previous algorithm we used
		// -1 to indicate that the clouds are not intersected by the photon
		// trajectory, we do not want to modify all the code now, that is
		// why we will work in a similar fashion and used -1
		phys_int bestIndex{ (distancePhotonToBestCloud != DBL_MAX) ? static_cast<phys_int>(bestGPUIndex) : -1 };

		// we check the best candidate from the cpu, we compare the results with
		// the data obtained in the previous for loop
		if (bestCPUIndex != -1)
		{
			auto cloudPosition{ m_cloudsPositions[bestCPUIndex] };
			auto currentDistance{ (photonPosition - cloudPosition).norm() };
			if (currentDistance < distancePhotonToBestCloud)
			{
				// we do not need to set:	distancePhotonToBestCloud = currentDistance
				// because we care only of the index
				bestIndex = bestCPUIndex;
			}
		}

		return bestIndex;

#else
		return -1;
#endif // PHYSAPI_USE_GPU

	}


	void PAGNClumpyCloudFinderGPU::initializeGPU(const sPosition* clouds_h)
	{
#ifdef PHYSAPI_USE_GPU
		cudaMalloc(&m_cloudPos_device, m_cloudsPositions.size() * sizeof(sPosition));
		cudaMemcpy(m_cloudPos_device, clouds_h, m_cloudsPositions.size() * sizeof(sPosition), cudaMemcpyHostToDevice);

		cudaMalloc(&m_indexes_device, m_gpuThreadsOrganizer.getNumOfGPUBlocks() * sizeof(phys_size));
		cudaMemcpy(m_indexes_device, m_indexes_host, m_gpuThreadsOrganizer.getNumOfGPUBlocks() * sizeof(phys_size), cudaMemcpyHostToDevice);
#endif // PHYSAPI_USE_GPU

	}
	void PAGNClumpyCloudFinderGPU::searchForCloudsGPU(const PSimplePhoton& photon)
	{
#ifdef PHYSAPI_USE_GPU
		// first get the position and direction(unit-vector-omega) of the photon
		auto x = photon.position().x();
		auto y = photon.position().y();
		auto z = photon.position().z();
		phys_float omega_x = photon.omega().x();
		phys_float omega_y = photon.omega().y();
		phys_float omega_z = photon.omega().z();

		for (phys_size i = 0; i < m_gpuThreadsOrganizer.getNumOfGPUBlocks(); i++)
		{
			m_indexes_host[i] = ULLONG_MAX;
		}

		cudaMemcpyAsync(m_indexes_device, m_indexes_host, m_gpuThreadsOrganizer.getNumOfGPUBlocks() * sizeof(phys_size), cudaMemcpyHostToDevice, m_cudaStream);
		//cudaMemcpy(m_indexes_device, m_indexes_host, m_gpuThreadsOrganizer.getNumOfGPUBlocks() * sizeof(phys_size), cudaMemcpyHostToDevice);
		// now we use an external function, which will launch the kernels, that search for the clouds
		// that are intersected by the photon trajectory, among those that correspond to gpu, because
		// notice that some o them are processed inside the cpu
		callMainAlgorithGPU(m_cloudPos_device, m_cloudsRadius, x, y, z, omega_x, omega_y, omega_z, m_indexes_device, m_gpuThreadsOrganizer.getNumOfGPUBlocks(), m_gpuThreadsOrganizer.getNumOfGPUThreadsPerBlock(), m_cudaStream);

		// now we "Wait for compute device(ie the gpu) to finish"
		//cudaDeviceSynchronize();

		// we then copy the array of indexes of the best candidates from the gpu to the cpu
		// so that we can analyse the results(examine the different candidates, ie the those
		// clouds that are intersected by the photon trajectory "first"), and then choose
		// which will be the first cloud intersected by the photon 
		cudaMemcpy(m_indexes_host, m_indexes_device, m_gpuThreadsOrganizer.getNumOfGPUBlocks() * sizeof(phys_size), cudaMemcpyDeviceToHost);
		//cudaMemcpyAsync(m_indexes_host, m_indexes_device, m_gpuThreadsOrganizer.getNumOfGPUBlocks() * sizeof(phys_size), cudaMemcpyDeviceToHost,m_cudaStream);
#endif // PHYSAPI_USE_GPU

	}
}// namespace agn