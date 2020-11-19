#pragma once

#include "utils.hpp"
#include "PSimplePhoton.hpp"
#include "PSimpleElectron.hpp"
#include "PMCCompton.hpp"

namespace physapi
{

    /**
     * @brief Perform Compton collision.
     * 
     * @tparam T_comptonCollision type of the collision
     * 
     * @ingroup compton
     */
    template <typename T_comptonCollision>
    class PComptonCollider
    {
    public:
        /**
         * @brief Construct a new PComptonCollider object
         * 
         */
        explicit PComptonCollider() {}

        /**
         * @brief Destroy the PComptonCollider object
         * 
         */
        virtual ~PComptonCollider() {}

        /**
         * @brief Perform the collision and set the corresponding momentum
         * to the photon and to the electron given as arguments
         * 
         * @param photon 
         * @param electron 
         */
        virtual void run(PSimplePhoton &photon, PSimpleElectron &electron) const
        {
            T_comptonCollision collisionHnd{photon, electron};

            photon.setMomentum(collisionHnd.photonMomentum());

            electron.setMomentum(collisionHnd.electronMomentum());
        }
    };

    /**
     * @brief Template specialization of PComptonCollider for compton Monte Carlo simulation(PMCCompton).
     * 
     * @see PMCCompton
     * 
     * @ingroup compton
     * 
     * **Example:**
     * 
     * @code{.cpp}
     * 
      #include <iostream>
      #include "PComptonCollider.hpp"
      #include "PMCCompton.hpp"

      using namespace std;
      using namespace physapi;
      int main()
      {
      PComptonCollider<PMCCompton> comptonCollider;

      PSimplePhoton photon{100.0E3_eV, PVector3D(1, 0, 0)};
      
      cout << photon.energy() << endl;
      cout << photon.momentum() << endl;
      
      comptonCollider.run(photon, 100.0E3_eVToKelvin);
      
      cout << photon.energy() << endl;
      
      cout << photon.momentum() << endl;
      }
     * @endcode
     * 
     * output:
     * 
     * 1.60219e-14
     * 
     * (5.34433e-23,0,0)
     * 
     * 2.07994e-14
     * 
     * (6.20026e-23,2.91483e-23,-1.09325e-23)
     * 
     */
    template <>
    class PComptonCollider<PMCCompton>
    {
    public:
        /**
         * @brief Construct a new PComptonCollider object
         * 
         */
        explicit PComptonCollider() {}

        /**
         * @brief Destroy the PComptonCollider object
         * 
         */
        virtual ~PComptonCollider() {}

        /**
         * @brief Perform the collision and set the corresponding momentum
         * to the photon and to the electron given as arguments
         * 
         * @param photon photon
         * @param T_e   temperature of the electron
         */
        virtual void run(PSimplePhoton &photon, phys_float T_e) const
        {
            PMCCompton collisionHnd{photon, T_e};

            photon.setMomentum(collisionHnd.photonMomentum());
        }
    };

} // namespace physapi
