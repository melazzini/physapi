#pragma once
#include "agn_utils.hpp"

namespace agn
{
    template <phys_size N_intervals>
    class PAGNSmoothSpectrumComponents
    {
    private:
        phys_float m_zenith;
        phys_float m_E_low;
        phys_float m_E_upp;
        eSpectrumScale m_scale;
        phys_float m_angularInterval;

    public:
        PAGNSmoothSpectrumComponents(phys_float zenithAngle,
                                     phys_float E_low,
                                     phys_float E_upp,
                                     eSpectrumScale scale = eSpectrumScale::LIN,
                                     phys_float angularInterval = 10.0_deg)
            : m_zenith{zenithAngle},
              m_E_low{E_low}, m_E_upp{E_upp}, m_scale{scale}, m_angularInterval{angularInterval}
        {
        }

        virtual ~PAGNSmoothSpectrumComponents() {}

        void spectrumComponents(
            const std::vector<std::string> &photonsFiles,
            PSpectrum<N_intervals> &full,
            PSpectrum<N_intervals> &reflected,
            PSpectrum<N_intervals> &transmitted,
            std::map<eImportantFluorescentLines, std::unique_ptr<PSpectrum<N_intervals>>> &flLines)
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
                        // full spectrum
                        full.addPhoton(energy);

                        // reflected spectrum
                        if (eTypeOfAGNPhoton(round(type)) == eTypeOfAGNPhoton::REFLECTED)
                        {
                            reflected.addPhoton(energy);
                            auto line_v = static_cast<eImportantFluorescentLines>(line);
                            if (line_v == eImportantFluorescentLines::NONE)
                            {
                                continue;
                            }
                            else if (flLines.find(line_v) != flLines.end())
                            {
                                flLines[line_v]->addPhoton(energy);
                            }
                            else
                            {
                                flLines[line_v] = std::make_unique<PSpectrum<N_intervals>>(m_E_low, m_E_upp, m_scale);
                                flLines[line_v]->addPhoton(energy);
                            }
                        }
                        else
                        {
                            transmitted.addPhoton(energy);
                        }
                    }

                    numOfPhotonsProcessed++;
                }
                fin.clear();
                fin.close();
            }
        }
    };
} // namespace agn
