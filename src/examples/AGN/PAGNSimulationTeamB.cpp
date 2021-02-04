#include"PAGNSimulationTeamB.hpp"
#include"PAGNInitSpectrumDirectionFilter.hpp"

namespace agn
{
	PAGNSimulationTeamB::PAGNSimulationTeamB(
		const std::shared_ptr<PVernerTable1> vernerTable1,
		const std::shared_ptr<PVernerTable2> vernerTable2,
		const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
		const std::shared_ptr<PAbundanceTable> abundances,
		phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
		const std::shared_ptr<PAGNFormula> agnformula,
		const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter)
		: m_vernerTable1{ vernerTable1 }, m_vernerTable2{ vernerTable2 },
		m_fluorescenceTable{ fluorescenceTable }, m_abundances{ abundances },
		m_id{ id }, m_numOfPhotons{ numOfPhotons }, m_n_e{ n_e }, m_T_e{ T_e },
		m_formula{ agnformula },
		m_verner{ *m_abundances, *m_vernerTable1, *m_vernerTable2 },
		m_fluorescenceHnd{ m_fluorescenceTable }, m_simMng{},
		m_initSpectrum(m_formula->E_low(), m_formula->E_cut(), eSpectrumScale::LOG),
		m_dirFilter{ *initSpectrumDirFilter }
	{
		PSpectrumMaker<N_intervals, SPECTRUM_PRECISION> spectrumMaker;

		// initialize the intrinsic spectrum spectrum
		spectrumMaker(m_initSpectrum, numOfPhotons, *m_formula);

		if (m_id == 0)
		{

			//std::cout << "Simulation team created(main thread)!" << std::endl;
			std::cout << "number of photons per thread (main thread):  " << m_initSpectrum.count() << std::endl;
		}
		/*else
		{
			std::cout << "New thread created, with id:	" << m_id << std::endl;
		}*/
	}

	void PAGNSimulationTeamB::run(std::string_view pathToStorageFolder)
	{
		if (id() == 0)
		{
			std::cout << "simulation started!" << std::endl;
		}

		m_simMng.setFileName(std::string(pathToStorageFolder) + "thread_" + std::to_string(id()) + ".txt");

		for (phys_size i = 0; i < m_initSpectrum.N_intervals; i++)
		{
			if ((i % phys_size(0.001 * m_initSpectrum.N_intervals) == 0) && id() == 0)
			{
				std::cout << "Intervals simulated: "
					<< 100.0 * phys_float(i) / phys_float(m_initSpectrum.N_intervals)
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
				PSimplePhoton photon{ m_randomValuesMng.uniform_real(E_low_i, E_upp_i) * 1.0_eV,
									 m_randDirMng(m_dirFilter) };

				// we run the simulation only if the photon is inside
				// the agn "limits"
				while (!m_simMng.isGone())
				{
					phys_float distanceToBoundary_v{};
					auto optionalDistanceToBoundary{ distanceToBoundary(photon) };
					if (optionalDistanceToBoundary.has_value())
					{
						m_simMng.enteredGeometry(true);
						distanceToBoundary_v = optionalDistanceToBoundary.value();
					}
					else
					{
						m_simMng.registerPhoton(photon);
						break;
					}

					m_listOfAbsorbingElements.clear();

					m_sigmaVerner = m_verner(photon.energy(), m_listOfAbsorbingElements);

					m_sigmaKleinNishina = KleinNishina(photon.energy());

					m_sigmaTotal = m_sigmaVerner + m_sigmaKleinNishina;

					m_lambdaMean = lambdaMean(m_sigmaTotal);

					m_escapeProbability = escapeProbability(distanceToBoundary_v, m_lambdaMean);

					// is there any interaction?
					// yes: move the photon to the interaction point
					// no: move the photon out of the agn structure && continue

					if (m_randomValuesMng.random_0_1() > m_escapeProbability)
					{
						auto distanceToInteractionPoint{
							PAGNStructureModelB::distanceToInteractionPoint(m_lambdaMean,
																		m_escapeProbability,
																		m_randomValuesMng.random_0_1()) };
						photon.translate(distanceToInteractionPoint);
						m_simMng.photonInteracted(true);
					}
					else
					{
						photon.translate(distanceToBoundary_v + 100.0_cm);

						m_simMng.enteredGeometry(false); // ie, the photon leaved the geometry
						continue;
					}

					// if any interaction happens then we choose whether it is photoabsortion or compton scattering

					// if photoabsortion then:
					//      * we need the corresponding fluorescence line(if any exist)
					//      * we need to update the photon's energy and direction(or recreate a new photon accordingly)
					if (m_randomValuesMng.random_0_1() <= m_sigmaVerner / m_sigmaTotal)
					{
						auto currentFluorescentLine = fluorescentLine(m_listOfAbsorbingElements, m_sigmaVerner);

						m_simMng.setFluorescenceLine(currentFluorescentLine.value_or(t_fluorescentLine()));

						// if no fluorescence happens
						if (!currentFluorescentLine.has_value())
						{
							// the photon has been absorbed and no fluorescent photon was created

							m_simMng.photonHasBeenAbsorbed(true);
							continue;
						}
						else
						{
							// the photon had been absorbed and a fluorescent photon has been created

							PSimplePhoton newFluorescentPhoton{ currentFluorescentLine.value().second.lineEnergy() * 1.0_eV,
															   m_randDirMng(),
															   photon.position() };

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

	std::optional<t_fluorescentLine> PAGNSimulationTeamB::
		fluorescentLine(const std::vector<std::array<phys_float, ABSORBINGELEMENT_PROPERTIES>>& absorbingElements,
			phys_float sigmaVerner)
	{
		// we need to find out which element absorbed the photon

		// in order to find what element absorbed the photon
		// we will create a list of probabilities
		std::vector<phys_float> crossSections_P;
		for (auto&& absorbingElement : absorbingElements)
		{
			crossSections_P.push_back(absorbingElement[eAbsorbingElementsDescription::SIGMA] / sigmaVerner);
		}

		// this is the absorbing element
		phys_size indexOfAbsorbingElement{
			getRandomIndexMontecarlo(crossSections_P, m_randomValuesMng) };

		// absorbing element's atomic number
		phys_size Z = absorbingElements[indexOfAbsorbingElement][eAbsorbingElementsDescription::Z];

		// absorbing element's shell number
		phys_size Is = absorbingElements[indexOfAbsorbingElement][eAbsorbingElementsDescription::IS];

		// at this point we know precisely what element has absorbed the photon

		// we now can obtain the fluorescent(if any) line corresponding to the absorbing element
		auto line = m_fluorescenceHnd.run(eZ(Z), Is);
		if (line == t_fluorescentLine())
		{
			return std::nullopt;
		}
		else
		{
			return line;
		}
	}

}// namespace agn