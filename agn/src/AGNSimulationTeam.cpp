#include "AGNSimulationTeam.hpp"

namespace agn
{
    AGNSimulationTeam::AGNSimulationTeam(
        const AGNStructureModel &structureModel,
        phys_float n_e,
        phys_float T_e,
        phys_float numberOfPhotons,
        phys_size id)
        : m_id{id},
          m_structureModel{structureModel},
          m_n_e{n_e}, m_T_e{T_e},
          m_initSpectrum{AGNFormula::E_low, AGNFormula::E_cut, eSpectrumScale::LOG}
    {
        // initialize the intrinsic spectrum spectrum
        m_spectrumMaker(m_initSpectrum, numberOfPhotons, m_formula);
    }

    void AGNSimulationTeam::run(const std::string &pathToFolder)
    {
        if (m_id == 0)
        {
            std::cout << "now!" << std::endl;
        }

        m_simMng.setFileName(pathToFolder + "thread_" + std::to_string(m_id) + ".txt");

        for (phys_size i = 0; i < m_spectrumMaker.INTERVALS; i++)
        {

            if ((i % phys_size(0.001 * m_spectrumMaker.INTERVALS) == 0) and m_id == 0)
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

                auto omega = photon.omega();

                auto phi = omega.phi() / 1.0_deg;
                auto theta = omega.theta() / 1.0_deg;

                // we run the simulation only if the photon is inside
                // the agn "limits"
                while (!m_simMng.isGone())
                {

                    // is inside the agn structure?
                    // yes: => calculate the distance to boundary
                    // no : => will enter?
                    //         yes: => move the photon to the entering point && continue
                    //         no : => register the photon
                    phys_float distanceToBoundary{0};
                    if (m_structureModel.isInside(photon.position()))
                    {
                        distanceToBoundary = m_structureModel.distanceToBoundary(photon.position(), photon.omega());

                        // double check
                        if (distanceToBoundary < 0)
                        {
                            m_simMng.registerPhoton(photon);
                            break;
                        }
                    }
                    else
                    {
                        phys_float distanceToEnteringPoint{m_structureModel.distanceToEnteringPoint(photon.position(),
                                                                                                    photon.omega())};
                        // if the distanceToEnteringPoint >= 0 it means that the photon
                        // will enter the agn structure

                        if (distanceToEnteringPoint >= 0)
                        {
                            // we add 100 cm in order to be sure that the photon is inside the geometry
                            photon.translate(distanceToEnteringPoint + 100.0_cm);

                            auto omega = photon.omega();

                            auto phi = omega.phi() / 1.0_deg;
                            auto theta = omega.theta() / 1.0_deg;

                            // double check
                            if (!m_structureModel.isInside(photon.position()))
                            {
                                m_simMng.registerPhoton(photon);
                                break;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            m_simMng.registerPhoton(photon);
                            break;
                        }
                    }

                    m_listOfAbsorbingElements.clear();

                    m_sigmaVerner = m_verner(photon.energy(), m_listOfAbsorbingElements);

                    m_sigmaKleinNishina = KleinNishina(photon.energy());

                    m_sigmaTotal = m_sigmaVerner + m_sigmaKleinNishina;

                    m_lambdaMean = lambdaMean(m_sigmaTotal);

                    m_escapeProbability = escapeProbability(distanceToBoundary, m_lambdaMean);

                    // is there any interaction?
                    // yes: move the photon to the interaction point
                    // no: move the photon out of the agn structure && continue

                    if (m_randomValuesMng.random_0_1() > m_escapeProbability)
                    {
                        auto distanceToInteractionPoint{
                            m_structureModel.distanceToInteractionPoint(photon.position(),
                                                                        photon.omega(),
                                                                        m_lambdaMean,
                                                                        m_escapeProbability,
                                                                        m_randomValuesMng.random_0_1())};
                        photon.translate(distanceToInteractionPoint);
                        m_simMng.photonInteracted(true);
                    }
                    else
                    {
                        photon.translate(distanceToBoundary + 100.0_cm);

                        m_simMng.enterGeometry(false);
                        continue;
                    }

                    // if any interaction happens then we choose whether it is photoabsortion or compton scattering

                    // if photoabsortion then:
                    //      * we need the corresponding fluorescence line(if any exist)
                    //      * we need to update the photon's energy and direction(or recreate a new photon accordingly)
                    if (m_randomValuesMng.random_0_1() <= m_sigmaVerner / m_sigmaTotal)
                    {
                        auto currentFluorescentLine = fluorescentLine(m_listOfAbsorbingElements, m_sigmaVerner);

                        m_simMng.setFluorescenceLine(currentFluorescentLine);

                        // if no fluorescence happens
                        if (currentFluorescentLine == t_fluorescentLine())
                        {
                            // the photon had been absorbed and no fluorescent photon has been created

                            m_simMng.photonHasBeenAbsorbed(true);
                            continue;
                        }
                        else
                        {
                            // the photon had been absorbed and a fluorescent photon has been created

                            PSimplePhoton newFluorescentPhoton{currentFluorescentLine.second.lineEnergy() * 1.0_eV,
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

    t_fluorescentLine AGNSimulationTeam::fluorescentLine(const std::vector<std::array<phys_float, 4>> &absorbingElements,
                                                         phys_float sigmaVerner)
    {
        // we need to find out which element absorbed the photon

        // in order to find what element absorbed the photon
        // we will create a list of probabilities
        std::vector<phys_float> crossSections_P;
        for (auto &&absorbingElement : absorbingElements)
        {
            crossSections_P.push_back(absorbingElement[eAbsorbingElementsDescription::SIGMA] / sigmaVerner);
        }

        // this is the absorbing element
        phys_size indexOfAbsorbingElement{
            getRandomIndexMontecarlo(crossSections_P, m_randomValuesMng)};

        // absorbing element's atomic number
        phys_size Z = absorbingElements[indexOfAbsorbingElement][eAbsorbingElementsDescription::Z];

        // absorbing element's shell number
        phys_size Is = absorbingElements[indexOfAbsorbingElement][eAbsorbingElementsDescription::IS];

        // at this point we know precisely what element has absorbed the photon

        // we now can obtain the fluorescent(if any) line corresponding to the absorbing element
        return m_fluorescenceHnd.run(eZ(Z), Is);
    }

} // namespace agn