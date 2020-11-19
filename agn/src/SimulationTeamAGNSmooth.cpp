#include "SimulationTeamAGNSmooth.hpp"
#include <fstream>

namespace agn
{
    const std::string SimulationTeamAGNSmooth::FILE_LABEL = "AGNSmooth";

    SimulationTeamAGNSmooth::SimulationTeamAGNSmooth(const PGeometryBase &geometry,
                                                     phys_float n_e,
                                                     phys_float T_e,
                                                     phys_float numberOfPhotons,
                                                     phys_size id_)
        : m_id{id_}, m_geometry(geometry), m_n_e{n_e}, m_T_e{T_e},
          m_initSpectrum{AGNFormula::E_low, AGNFormula::E_cut, eSpectrumScale::LOG}
    {
        // initialize the intrinsic spectrum spectrum
        m_spectrumMaker(m_initSpectrum, numberOfPhotons, m_formula);
    }

    void SimulationTeamAGNSmooth::run(const std::string &pathToFolder)
    {
        if (m_id == 0)
        {
            std::cout << "now!" << std::endl;
        }

        m_simMng.setFileName(pathToFolder + "thread_" + std::to_string(m_id) + ".txt");

        // 3475
        for (phys_size i = 0; i < m_spectrumMaker.INTERVALS; i++)
        {

            if ((i % phys_size(0.01 * m_spectrumMaker.INTERVALS) == 0) and m_id == 0)
            {
                std::cout << "Intervals simulated: "
                          << 100.0 * phys_float(i) / phys_float(m_spectrumMaker.INTERVALS)
                          << "%     \r";
                std::cout << std::flush;
            }

            // number of photons in the current interval
            phys_float numberOfPhotons_i = m_initSpectrum.value(i);

            // energy bounds

            phys_float E_low_i = m_initSpectrum.subintervalEnergyLowerBound(i);
            phys_float E_upp_i = m_initSpectrum.subintervalEnergyUpperBound(i);

            for (phys_float j = 0; j < numberOfPhotons_i; j++)
            {

                // we reset the simulation manager, so that
                // it can start following the photon from its creation
                m_simMng.reset();

                // current photon
                PSimplePhoton photon{m_randomValuesMng.uniform_real(E_low_i, E_upp_i) * 1.0_eV,
                                     m_randDirMng(m_dirFilter)};

                // we run the simulation only if the photon is inside
                // the agn
                while (!m_simMng.isGone())
                {
                    if (!m_simMng.isInside())
                    {
                        if (m_geometry.isInside(photon.position()))
                        {
                            m_simMng.enterGeometry(true);
                        }
                        else
                        {
                            auto distanceToGeometry = m_geometry.distanceToBoundaryFromOutside(photon.position(), photon.omega());

                            if (distanceToGeometry >= 0)
                            {
                                auto pos = photon.position();

                                // we add 100 cm in order to take the photon into the geometry
                                auto newPosition = pos + (distanceToGeometry + 100.0_m) * photon.omega();

                                photon.setPosition(PPosition(newPosition));

                                // double check
                                if (m_geometry.isInside(photon.position()))
                                {
                                    m_simMng.enterGeometry(true);
                                }
                                else
                                {
                                    m_simMng.registerPhoton(photon);
                                    break;
                                }
                            }
                            else
                            {
                                m_simMng.registerPhoton(photon);
                                break;
                            }
                        }
                    }

                    // length of the photon trajectory inside the geometry
                    phys_float lengthInsideTheAGN = m_geometry.distanceToBoundaryFromInside(photon.position(),
                                                                                            photon.omega());

                    // length between boundaries, not inside the geometry
                    phys_float freeLength = m_geometry.freeLength(photon.position(),
                                                                  photon.omega());

                    m_listOfAbsorvingElements.clear();

                    m_sigmaVerner = m_verner(photon.energy(), m_listOfAbsorvingElements);

                    m_sigmaKleinNishina = KleinNishina(photon.energy());

                    m_sigmaTotal = m_sigmaVerner + m_sigmaKleinNishina;

                    if (m_sigmaTotal > 0)
                    {
                        m_lambdaMean = 1.0 / (m_n_e * m_sigmaTotal);

                        m_escapeProbability = std::exp(-lengthInsideTheAGN / m_lambdaMean);
                    }
                    else
                    {
                        m_lambdaMean = PLUS_INF;

                        m_escapeProbability = 1;
                    }

                    if (m_randomValuesMng.random_0_1() > m_escapeProbability)
                    {
                        movePhotonTointeractionPoint(photon, m_lambdaMean, m_escapeProbability, freeLength);
                        m_simMng.photonInteracted(true);
                    }
                    else
                    {
                        auto pos = photon.position();

                        // we add 100 cm in order to take the photon out
                        // of the geometry
                        auto newPosition = pos + (lengthInsideTheAGN + 100.0_m) * photon.omega();

                        // double check
                        if (m_geometry.isInside(photon.position()))
                        {
                            m_simMng.registerPhoton(photon);
                            break;
                        }
                        else
                        {
                            photon.setPosition(PPosition(newPosition));
                            m_simMng.enterGeometry(false);

                            continue;
                        }
                    }

                    // if any interaction happens then we choose whether it is photoabsortion or compton scattering

                    // if photoabsortion then:
                    //      * we need to find out which element absorbed the photon
                    //      * we need the corresponding fluorescence line(if any exist)
                    //      * we need to update the photon's energy and direction(or recreate a new photon accordingly)
                    if (m_randomValuesMng.random_0_1() <= m_sigmaVerner / m_sigmaTotal)
                    {
                        // in order to find what element absorbed the photon
                        // we will create a list of probabilities
                        std::vector<phys_float> crossSections_P;
                        for (auto &&absorbingElement : m_listOfAbsorvingElements)
                        {
                            crossSections_P.push_back(absorbingElement[eAbsorbingElementsDescription::SIGMA] / m_sigmaVerner);
                        }

                        // this is the absorbing element
                        phys_size indexOfAbsorbingElement{
                            getRandomIndexMontecarlo(crossSections_P, m_randomValuesMng)};

                        // absorbing element's atomic number
                        phys_size Z = m_listOfAbsorvingElements[indexOfAbsorbingElement][eAbsorbingElementsDescription::Z];

                        // absorbing element's shell number
                        phys_size Is = m_listOfAbsorvingElements[indexOfAbsorbingElement][eAbsorbingElementsDescription::IS];

                        // at this point we know precisely what element has absorbed the photon

                        // we now can obtain the fluorescent(if any) line corresponding to the absorbing element
                        auto fluorescentLine = m_fluorescenceHnd.run(eZ(Z), Is);

                        m_simMng.setFluorescenceLine(fluorescentLine);

                        // if no fluorescence happens
                        if (fluorescentLine == t_fluorescentLine())
                        {
                            // the photon had been absorbed and no fluorescent photon has been created

                            m_simMng.photonHasBeenAbsorbed(true);
                            continue;
                        }
                        else
                        {
                            // the photon had been absorbed and a fluorescent photon has been created

                            PSimplePhoton newFluorescentPhoton{fluorescentLine.second.lineEnergy() * 1.0_eV,
                                                               m_randDirMng(),
                                                               photon.position()};

                            photon = newFluorescentPhoton;
                        }
                    }
                    // if compton scattering happens then we perform the corresponding montecarlo simulation
                    else
                    {
                        m_comptonHnd.run(photon, m_T_e);
                    }
                }
            }
        }
    }

    void SimulationTeamAGNSmooth::movePhotonTointeractionPoint(PSimplePhoton &photon,
                                                               phys_float lambdaMean,
                                                               phys_float escapeProbability,
                                                               phys_float freeLength)
    {
        phys_float freePath = -lambdaMean * std::log(1 - m_randomValuesMng.random_0_1() * (1 - escapeProbability)) + freeLength;

        PVector3D dr = freePath * photon.omega();

        PVector3D r = photon.position();

        photon.setPosition(static_cast<PPosition>(r + dr));
    }

} // namespace agn
