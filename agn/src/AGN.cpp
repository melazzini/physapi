#include "AGN.hpp"
#include <thread>
#include "AGNSimulationTeam.hpp"

namespace agn
{
    AGN::AGN(const AGNStructureModel &structureModel,
             phys_float Te,
             phys_float numOfPhotons)
        : m_structureModel{structureModel},
          m_NH{N_H}, m_Te{Te},
          m_numOfPhotons{numOfPhotons},
          m_n_e{calculate_n_e(m_structureModel.n_H())}
    {
    }

    void AGN::run(const std::string &pathToFolder, eNumOfThreads numOfThreads)
    {
        auto numOfHardwareThreads = (numOfThreads == eNumOfThreads::MAX)
                                        ? std::thread::hardware_concurrency()
                                        : static_cast<phys_size>(numOfThreads);

        phys_float numOfPhotonsPerThread = m_numOfPhotons / static_cast<phys_float>(numOfHardwareThreads);

        std::cout << "photons per thread:    " << numOfPhotonsPerThread << std::endl;

        std::vector<std::thread> threads;

        for (phys_size i = MAINTHREAD_ID + 1; i < numOfHardwareThreads; i++)
        {
            std::cout << "New thread created! ID: "
                      << "#" << i
                      << std::endl;

            threads.push_back(std::thread(&AGN::creatSimulationThread, this,
                                          i,
                                          numOfPhotonsPerThread,
                                          pathToFolder));
        }

        AGNSimulationTeam mainTeam{m_structureModel, m_n_e, m_Te,
                                   numOfPhotonsPerThread,
                                   MAINTHREAD_ID};

        mainTeam.run(pathToFolder);

        for (auto &&thread_ : threads)
        {
            thread_.join();
        }
    }

    phys_float AGN::calculate_n_e(phys_float n_H) const
    {
        return m_abundances.total() * n_H;
    }

    void AGN::creatSimulationThread(phys_size id,
                                    phys_float numOfPhotons,
                                    const std::string &pathToFolder)
    {
        AGNSimulationTeam team{
            m_structureModel,
            m_n_e,
            m_Te,
            numOfPhotons,
            id};

        team.run(pathToFolder);
    }
} // namespace agn
