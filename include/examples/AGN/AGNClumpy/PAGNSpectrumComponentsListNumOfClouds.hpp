#pragma once
#include "agn_utils.hpp"
#include <thread>

namespace agn
{
    template <phys_size N_intervals>
    class PAGNSpectrumComponentsListNumOfClouds
    {
    private:
        phys_float m_zenith;
        phys_float m_E_low;
        phys_float m_E_upp;
        std::string m_cloudsFile;
        phys_float m_cloudsRadius;
        eSpectrumScale m_scale;
        phys_float m_angularInterval;
        t_clouds m_clouds;
        std::vector<t_clouds> m_cloudsVector;
        std::map<phys_size, std::unique_ptr<PSpectrum<N_intervals>>> m_fullSpectrumList;

    public:
        PAGNSpectrumComponentsListNumOfClouds(phys_float zenithAngle,
                                              phys_float E_low,
                                              phys_float E_upp,
                                              const std::string cloudsFile,
                                              phys_float cloudsRadius,
                                              eSpectrumScale scale = eSpectrumScale::LIN,
                                              phys_float angularInterval = 10.0_deg) : m_zenith{zenithAngle}, m_E_low{E_low}, m_E_upp{E_upp},
                                                                                       m_cloudsFile{cloudsFile}, m_cloudsRadius{cloudsRadius},
                                                                                       m_scale{scale}, m_angularInterval{angularInterval},
                                                                                       m_clouds{}
        {
            loadClouds();
        }

        void loadElement(const std::array<phys_float, 3> &cloudCoordinates)
        {
            m_clouds.push_back(PPosition{cloudCoordinates[0],   //x
                                         cloudCoordinates[1],   //y
                                         cloudCoordinates[2]}); //z
        }

        void
        spectrumLists(const std::vector<std::string> &photonsFiles,
                      agnSpectrumList_t<N_intervals> &fullSpectrumList,
                      agnSpectrumList_t<N_intervals> &reflectedSpectrumList,
                      agnSpectrumList_t<N_intervals> &transmittedSpectrumList,
                      agnFlLinesSpectrumList_t<N_intervals> &flLinesSpectrumList)
        {
            phys_float minPhi = Pi / 2 - (m_zenith + m_angularInterval);
            phys_float maxPhi = Pi / 2 - m_zenith;
            phys_size numOfPhotonsProcessed{};

            std::ifstream fin;
            for (auto &&file_i : photonsFiles)
            {
                fin.open(file_i);
                if (!fin.good())
                {
                    std::cerr << "Error while opening the file:	" << file_i << std::endl;
                    std::abort();
                }

                phys_float energy{}, theta{}, phi{}, type{}, line{};

                while (fin >> energy)
                {
                    fin >> theta;
                    fin >> phi;
                    fin >> type;
                    fin >> line;

                    if (numOfPhotonsProcessed % 1'000 == 0)
                    {
                        std::cout << "file:	" << file_i << std::endl;
                        std::cout << "Number of photons processed(AGN_FLUX NUM_OF_CLOUDS):    "
                                  << numOfPhotonsProcessed << std::endl;
                    }

                    if (checkRangeInclusive(minPhi, std::abs(phi), maxPhi))
                    {
                        auto numOfClouds = (phys_size(type) == 0) ? 0 : countClouds(theta, phi);

                        // full spectrum
                        addPhoton(fullSpectrumList, numOfClouds, energy);

                        // reflected spectrum
                        if (eTypeOfAGNPhoton(round(type)) == eTypeOfAGNPhoton::REFLECTED)
                        {
                            addPhoton(reflectedSpectrumList, numOfClouds, energy);

                            auto line_v = static_cast<eImportantFluorescentLines>(line);
                            if (line_v == eImportantFluorescentLines::NONE)
                            {
                                continue;
                            }
                            else if (flLinesSpectrumList.find(line_v) != flLinesSpectrumList.end())
                            {
                                addPhoton(flLinesSpectrumList[line_v], numOfClouds, energy);
                            }
                            else
                            {
                                flLinesSpectrumList[line_v] = agnSpectrumList_t<N_intervals>();
                                addPhoton(flLinesSpectrumList[line_v], numOfClouds, energy);
                            }
                        }
                        // transmitted spectrum
                        else
                        {
                            addPhoton(transmittedSpectrumList, numOfClouds, energy);
                        }
                    }
                    numOfPhotonsProcessed++;
                }
                fin.clear();
                fin.close();
            }
        }

    private:
        void loadClouds()
        {

            std::ifstream fin;
            fin.open(m_cloudsFile);
            if (!fin.good())
            {
                std::cout << "File:   " << m_cloudsFile
                          << "  not found!" << std::endl;
                std::abort();
            }

            phys_float x{}, y{}, z{};

            const phys_size numOfThreads{std::thread::hardware_concurrency()};

            m_cloudsVector.resize((numOfThreads >= 1) ? numOfThreads : 1);

            phys_size threadIndex{};

            while (fin >> x)
            {
                fin >> y;
                fin >> z;

                m_cloudsVector[threadIndex].push_back({x, y, z});

                threadIndex++;

                if (numOfThreads == threadIndex)
                {
                    threadIndex = 0;
                }
            }
            fin.clear();
            fin.close();
        }

        phys_size countClouds(phys_float theta, phys_float phi)
        {
            PVector3D dir{1, 0, 0};
            dir.setPhi(phi);
            dir.setTheta(theta);

            std::vector<std::thread> threads;

            std::vector<phys_size> numOfCloudsVector(m_cloudsVector.size());

            for (phys_size i = 0; i < m_cloudsVector.size() - 1; i++)
            {
                threads.push_back(std::thread(&PAGNSpectrumComponentsListNumOfClouds::countAlgorithm, this, std::ref(dir),
                                              i,
                                              std::ref(numOfCloudsVector[i])));
            }

            // main thread starts to count the clouds here
            countAlgorithm(dir, m_cloudsVector.size() - 1, numOfCloudsVector[m_cloudsVector.size() - 1]);

            for (auto &&thread_i : threads)
            {
                thread_i.join();
            }

            return std::accumulate(numOfCloudsVector.begin(), numOfCloudsVector.end(), 0);
        }

        void countAlgorithm(const PVector3D &dir, phys_size threadIndex, phys_size &numOfClouds)
        {
            //this is the angle between the given
            //direction and the cloud's radius vector
            phys_float Theta{};

            // number of clouds found on the given direction
            phys_size numberOfCloudsFound{};

            // distance to the line, defined by the
            // given direction
            phys_float d{};

            for (phys_size i = 0; i < m_cloudsVector[threadIndex].size(); i++)
            {
                // we need to know the angle between the given direction
                // and the radius vector of the current cloud
                Theta = PVector3D::angleBetweenVectors(dir, m_cloudsVector[threadIndex][i]);

                //if the cloud is "behind" the given direction we discard it
                if (Theta >= (Pi / 2.0))
                {
                    continue;
                }

                // the distance to the trajectory is triangle leg:
                d = m_cloudsVector[threadIndex][i].norm() * std::sin(Theta);

                //if the distance from the cloud to the line
                //is less than the cloud's radius we count this cloud
                if (d < m_cloudsRadius)
                {
                    numberOfCloudsFound++;
                }
            }

            numOfClouds = numberOfCloudsFound;
        }

        void addPhoton(agnSpectrumList_t<N_intervals> &spectrumList, phys_size numOfClouds, phys_float hv)
        {
            if (spectrumList.find(numOfClouds) != spectrumList.end())
            {
                spectrumList[numOfClouds]->addPhoton(hv);
            }
            else
            {
                spectrumList[numOfClouds] = std::make_unique<PSpectrum<N_intervals>>(
                    m_E_low, m_E_upp, m_scale);

                spectrumList[numOfClouds]->addPhoton(hv);
            }
        }
    };
} // namespace agn
