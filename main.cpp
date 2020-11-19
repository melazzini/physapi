#include "AGN.hpp"
#include "AGNSmoothTorusModel.hpp"
#include "AGNClumpyTorusModel.hpp"
#include "AGNSmoothSphereModel.hpp"
#include <iostream>
#include "PFluxPerEnergyIntervalMaker.hpp"
#include <chrono>
#include <thread>

using namespace std;
using namespace agn;
using namespace physapi;

const string FILES_PREFIX = "./data/thread_";
const string FILES_EXTENSION = ".txt";

int main()
{
    // AGNSmoothTorusModel torus{PSimpleTorus(TORUS_R1, TORUS_R2, TORUS_THETA0), N_H};
    // AGNSmoothSphereModel sphere{PSphere(PPosition(), SPHERE_R), N_H};
    AGNClumpyTorusModel clumpyTorus{PSimpleTorus(TORUS_R1, TORUS_R2, TORUS_THETA0), N_H, 4, 0.5};

    AGN agn{clumpyTorus, T_e, NUMBER_OF_PHOTONS};

    agn.run(PATH_STORAGE_DATA, eNumOfThreads::TWO);

    return 0;
}
