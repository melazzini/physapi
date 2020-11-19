#pragma once

#include "SimulationTeamAGNSmooth.hpp"

namespace agn
{
    /**
     * @brief This class represents an agn with an homogeneous
     * and isotropic internal structure.
     * 
     * @ingroup agn
     * 
     */
    class AGNSmooth
    {
        // ID of the main thread
        static constexpr phys_size MAINTHREAD_ID = 0;

    public:
        /**
         * @brief Construct a new smooth agn
         * 
         * @param geometry 
         */
        AGNSmooth(const PGeometryBase &geometry, phys_float N_H, phys_float T_e, phys_float numOfPhotons);

        /**
         * @brief Destroy the agn
         * 
         */
        virtual ~AGNSmooth() {}

        /**
         * @brief Run the agn x-ray spectral model simulation
         * 
         * @param pathToStorageFolder 
         */
        void run(const std::string &pathToStorageFolder);

    private:
        const PGeometryBase &m_geometry; // geometry of the agn

        const phys_float m_N_H; // equatorial average column density

        const phys_float m_T_e; // temperature of electrons

        const phys_float m_numOfPhotons; // number of photons in the simulation(only those which enter the geometry!)

        PAbundanceTable m_abundances; // abundance table

        const phys_float m_n_H; // Hydrogen concentration

        const phys_float m_n_e; // electron concentration

    private:
        // calculate concentration of hydrogen
        phys_float calculate_n_H(phys_float N_H, phys_float equatorialSize) const;

        // calculate concentration of electrons
        phys_float calculate_n_e(phys_float n_H) const;

        void creatSimulationThread(phys_size id, phys_float numOfPhotons, const std::string &pathToFolder);
    };
} // namespace agn
