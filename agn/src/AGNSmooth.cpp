#include "AGNSmooth.hpp"
#include <thread>
#include "SimulationTeamAGNSmooth.hpp"

namespace agn
{
    AGNSmooth::AGNSmooth(const PGeometryBase &geometry,
                         phys_float N_H,
                         phys_float T_e,
                         phys_float numOfPhotons)
        : m_geometry(geometry), m_N_H{N_H}, m_T_e{T_e},
          m_numOfPhotons{numOfPhotons},
          m_n_H{calculate_n_H(m_N_H, m_geometry.estimationAverageSize())},
          m_n_e{calculate_n_e(m_n_H)}
    {
    }

    void AGNSmooth::run(const std::string &pathToStorageFolder)
    {
        auto numOfHardwareThreads = 2;// std::thread::hardware_concurrency();

        phys_float numOfPhotonsPerThread = m_numOfPhotons / static_cast<phys_float>(numOfHardwareThreads);

        std::cout << "photons per thread:    " << numOfPhotonsPerThread << std::endl;

        std::vector<std::thread> threads;

        for (phys_size i = MAINTHREAD_ID + 1; i < numOfHardwareThreads; i++)
        {
            std::cout << "New thread created! ID: "
                      << "#" << i
                      << std::endl;
            threads.push_back(std::thread(&AGNSmooth::creatSimulationThread, this,
                                          i,
                                          numOfPhotonsPerThread,
                                          pathToStorageFolder));
        }

        std::cout << "Main thread running! ID: " << MAINTHREAD_ID << std::endl;

        SimulationTeamAGNSmooth mainTeam{m_geometry,
                                         m_n_e, m_T_e,
                                         numOfPhotonsPerThread,
                                         MAINTHREAD_ID};

        mainTeam.run(pathToStorageFolder);

        for (auto &&thread_ : threads)
        {
            thread_.join();
        }
    }

    phys_float AGNSmooth::calculate_n_H(phys_float N_H, phys_float equatorialSize) const
    {
        if (equatorialSize <= 0)
        {
            std::cerr << "Bad equatorial size!" << std::endl;
            std::abort();
        }

        return N_H / (equatorialSize);
    }

    phys_float AGNSmooth::calculate_n_e(phys_float n_H) const
    {
        return m_abundances.total() * n_H;
    }

    void AGNSmooth::creatSimulationThread(phys_size id, phys_float numOfPhotons, const std::string &pathToFolder)
    {
        SimulationTeamAGNSmooth team{
            m_geometry,
            m_n_e,
            m_T_e,
            numOfPhotons,
            id};

        team.run(pathToFolder);
    }

} // namespace agn
