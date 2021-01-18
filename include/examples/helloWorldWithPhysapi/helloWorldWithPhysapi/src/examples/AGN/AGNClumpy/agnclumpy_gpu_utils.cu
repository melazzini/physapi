
#define CUDA_API_PER_THREAD_DEFAULT_STREAM

#include<cuda.h>
#include "device_launch_parameters.h"
#include <device_functions.h>
#include<cuda_runtime.h>
#include "PAGNClumpyCloudFinderGPU.hpp"

namespace agn
{
	__global__ void main_algorithm_gpu(sPosition* cloud_d, phys_float R, phys_float x, phys_float y, phys_float z, phys_float omega_x, phys_float omega_y, phys_float omega_z, phys_size* index)
	{
		// we need the index of the current cloud
		auto indexCloud = blockIdx.x * blockDim.x + threadIdx.x;

		//auto indexCloud=0;

		//---ALGORITHM STARTS HERE--------------------------------------------------------------------------

		//initialy we get the vector from the photon
		//to the cloud: r_cloud - r_photon
		auto x_photonToCloud{ cloud_d[indexCloud].x - x };
		auto y_photonToCloud{ cloud_d[indexCloud].y - y };
		auto z_photonToCloud{ cloud_d[indexCloud].z - z };

		// we need the dot product to get the angle theta(see below)
		auto xyz_photonToCloud_Times_Omega = x_photonToCloud * omega_x + y_photonToCloud * omega_y + z_photonToCloud * omega_z;

		// we need the norm of the vectors
		auto r_photonToCloud = ::sqrt(x_photonToCloud * x_photonToCloud + y_photonToCloud * y_photonToCloud + z_photonToCloud * z_photonToCloud);
		auto omega = ::sqrt(omega_x * omega_x + omega_y * omega_y + omega_z * omega_z);

		// now we can get the angle between the radius vector of the cloud
		// and the dicrection(omega) of the photon
		auto theta = std::acos(xyz_photonToCloud_Times_Omega / (r_photonToCloud * omega));

		// this variable has a special meaning in this algorithm:
		// if the cloud is intersectec by the photon trajectory
		// then its value is the distance from the cloud to the photon
		// otherwise it is the max value the corresponding type
		phys_size distanceCloudToPhoton_here{ ULLONG_MAX };

		// only if the cloud is not "behind" the photon we calculate the distance
		// from the center of the cloud to the trajectory of the photon
		if (theta < (3.141592653589 / 2.0))
		{
			// this is the distance from the center of the cloud to
			// the photon trajectory
			auto distance_value = r_photonToCloud * ::sin(theta);

			// and only if the distance value is less than its radius
			// we set the variable distance_here to the value of
			// the distance from the photon to the cloud center type
			// casted to the corresponding interger value in order
			// to be able to use cuda atomic operations below
			if (distance_value < R)
			{
				distanceCloudToPhoton_here = __double2ull_rn(r_photonToCloud);
			}
			else
				return;
		}
		else
			return;

		//--- ALGORITHM FINISHES HERE--------------------------------------------------------------------------

		// at this point we have the distance to the photon from
		// the cloud, it will be THE MAX_VAL if the current cloud is not
		// intersected by the photon trajectory, otherwise it will be the real
		// distance BUT IN UNSIGNED LONG LONG-TYPE!!!

		// THE REMAINING PART OF THE CODE CONCERNS THE FINDING OF
		// THE FIRST CLOUD THAT WILL BE INTERSECTED BY THE PHOTON
		// IF THERE IS AT LEAST ONE IN THE CORRESPONDING THREAD BLOCK

		// now we need a shared variable that will represent the distance
		// to the best candidate, among all the clouds in the current block
		__shared__ phys_size distanceToBestCandidate;

		// we set the value of the distance to the max possible value
		// width the corresponding type
		distanceToBestCandidate = ULLONG_MAX;

		// we wait until all threads get to this point and the value
		// of distance_here is properly set
		__syncthreads();

		// if the current cloud is intersected by the photon's trajectory
		// then its distance to the photon will be less than the init value
		// ie less than the ULLONG_MAX in our algorithm
		if (distanceCloudToPhoton_here < ULLONG_MAX)
		{
			// because the value of distance will be compared with the other
			// values of others threads, one by one we use atomicMin()
			::atomicMin(&distanceToBestCandidate, distanceCloudToPhoton_here);
		}

		// we wait untill the final value of distanceToBestCandidate is set,
		// this can be one of two values: the distance from the photon to the first
		// cloud that it will intersect or the max value initially set, if the photon
		// will not intersect any cloud at all
		__syncthreads();

		// now we can comprare if the distance of the current cloud to
		// the photon is the distance from the photon to the first
		// cloud that will be intersected. on the other hand it is possible
		// that the photon wont intersect any cloud, in that case
		// the variable distanceToBestCandidate will be the initial, ie the max value
		// so its important to check that condition also
		// to sumarise: only if there is a cloud that will be intersected
		// by the photon first, and this thread corresponds to that cloud
		// we set the index of that cloud in the general array of clouds
		// to the block of candidates

		if (distanceToBestCandidate != ULLONG_MAX && distanceToBestCandidate == distanceCloudToPhoton_here)
		{
			index[blockIdx.x] = indexCloud;
		}
	}

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
		main_algorithm_gpu << <NUM_OF_BLOCKS, NUM_OF_THREADS_PER_BLOCK >> > (cloud_d, R, x, y, z, omega_x, omega_y, omega_z, indexes_d);
		//foo2 << < NUM_OF_BLOCKS, NUM_OF_THREADS_PER_BLOCK >> > ();
	}

}// namespace agn