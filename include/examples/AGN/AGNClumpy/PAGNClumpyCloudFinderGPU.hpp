#pragma once
#include "PAGNClumpyCloudFinderB.hpp"
#include "agn_utils.hpp"
#include <algorithm>
#include <thread>
#include<optional>
#define CUDA_API_PER_THREAD_DEFAULT_STREAM

#include<cuda.h>
#include "device_launch_parameters.h"
#include <device_functions.h>
#include<cuda_runtime.h>

namespace agn
{

	// this function launches the kernels that search for the clouds that are intersected
	// by the photon trajactory
	void callMainAlgorithGPU(
		//array of pos of the clouds and the radius of the clouds
		sPosition* cloudPositions_device, phys_float RadiusOfTheClouds,

		//position of the photon
		phys_float x, phys_float y, phys_float z,

		//direction of the photon
		phys_float omega_x, phys_float omega_y, phys_float omega_z,

		//array of indexes of the best candidates(one for each gpu-block)
		phys_size* indexes_d,

		//num of gpu blocks and num of gpu threads per each block
		phys_size NUM_OF_BLOCKS, phys_size NUM_OF_THREADS_PER_BLOCK, cudaStream_t s);


	//This class finds the next cloud,
	//which is intersected by a
	//photon on its trajectory, if
	//there exists such a cloud.
	//Dont use this class if the current
	//photon is inside a cloud already!
	class PAGNClumpyCloudFinderGPU
	{
		class _ThreadsOrganizer
		{

		public:
			static constexpr phys_size NUM_OF_THREADS_PER_GPUBLOCK = 32;

			_ThreadsOrganizer(phys_size numOfClouds)
			{
				m_numOfGPUBlocks = numOfClouds / NUM_OF_THREADS_PER_GPUBLOCK;
				m_numOfCPUClouds = numOfClouds % NUM_OF_THREADS_PER_GPUBLOCK;
			}

			phys_size getNumOfGPUBlocks()const { return m_numOfGPUBlocks; }

			phys_size getNumOfGPUThreadsPerBlock()const { return NUM_OF_THREADS_PER_GPUBLOCK; }

			phys_size getNumOfCPUClouds()const { return m_numOfCPUClouds; }

		private:
			phys_size m_numOfGPUBlocks;
			phys_size m_numOfCPUClouds;
		};

	public:
		PAGNClumpyCloudFinderGPU(const std::vector<PPosition>& cloudsPositions,
			phys_float cloudsRadius);

		phys_int operator()(const PSimplePhoton& photon) ;

	private:
		const std::vector<PPosition>& m_cloudsPositions;

		phys_float m_cloudsRadius;

		cudaStream_t m_cudaStream;

		_ThreadsOrganizer m_gpuThreadsOrganizer;

		phys_size* m_indexes_host{ nullptr };
		phys_size* m_indexes_device{ nullptr };

		sPosition* m_cloudPos_device{ nullptr };

	private:

		phys_int mainAlgorithm(const PSimplePhoton& photon,
			const std::vector<PPosition>& cloudsPositions,
			phys_float cloudsRadius,
			phys_size firstIndex = 0// this parameter helps us to work with the gpu-cpu
		);

		void initializeGPU(const sPosition* clouds_h);

		void searchForCloudsGPU(const PSimplePhoton& photon);
	};
}// namespace agn