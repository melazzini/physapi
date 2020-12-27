
#define CUDA_API_PER_THREAD_DEFAULT_STREAM

#include<cuda.h>
#include "device_launch_parameters.h"
#include <device_functions.h>
#include<cuda_runtime.h>
#include "PAGNClumpyCloudFinderGPU.hpp"

namespace agn
{
	__global__ void foo2()
	{
		int i = threadIdx.x + 1 * 3;
	}

	void callMainAlgorithGPU(sPosition* cloud_d, phys_float R,
		phys_float x, phys_float y, phys_float z,
		phys_float omega_x, phys_float omega_y, phys_float omega_z,
		phys_size* indexes_d,
		phys_size NUM_OF_BLOCKS, phys_size NUM_OF_THREADS_PER_BLOCK, cudaStream_t s)
	{
		foo2 << < NUM_OF_BLOCKS, NUM_OF_THREADS_PER_BLOCK >> > ();
	}

}// namespace agn