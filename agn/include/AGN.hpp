#pragma once

#include "AGNStructureModel.hpp"

namespace agn
{
    /**
     * @brief This class represents an AGN
     * 
     * @ingroup agn
     * 
     */
    class AGN
    {
        // ID of the main thread
        static constexpr phys_size MAINTHREAD_ID = 0;

    private:
        // abundance table
        PAbundanceTable m_abundances;

        // structural model
        const AGNStructureModel &m_structureModel;

        // average column density along the equator
        phys_float m_NH;

        // temperature of the electrons
        phys_float m_Te;

        // number of photons in the simulation
        phys_float m_numOfPhotons;

        // concentration of hydrogen
        phys_float m_n_H;

        // concentration of electrons
        phys_float m_n_e;

    public:
        /**
         * @brief Construct a new AGN object
         * 
         * @param structureModel model of the agn structure
         */
        AGN(const AGNStructureModel &structureModel,
            phys_float Te,
            phys_float numOfPhotons);

        /**
         * @brief Run the simulation of the agn x-ray spectrum
         * 
         * @param PATH_TO_FOLDER path to storage folder
         * @param numOfThreads number of threads in the simulation
         */
        void run(const std::string &PATH_TO_FOLDER, eNumOfThreads numOfThreads = eNumOfThreads::MAX);

    private:

        // calculate concentration of electrons
        phys_float calculate_n_e(phys_float n_H) const;

        void creatSimulationThread(phys_size id, phys_float numOfPhotons, const std::string &pathToFolder);
    };
} // namespace agn
