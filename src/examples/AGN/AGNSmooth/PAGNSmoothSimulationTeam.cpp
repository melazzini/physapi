#include"PAGNSmoothSimulationTeam.hpp"

namespace agn
{
	PAGNSmoothSimulationTeam::PAGNSmoothSimulationTeam(
		const std::shared_ptr<PAGNStructureModelB> structureModel,
		const std::shared_ptr<PVernerTable1> vernerTable1,
		const std::shared_ptr<PVernerTable2> vernerTable2,
		const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
		const std::shared_ptr<PAbundanceTable> abundances,
		phys_size id, phys_float numOfPhotons, phys_float n_e,
		const std::shared_ptr<PAGNFormula> agnformula,
		const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter)
		: PAGNSimulationTeamB(vernerTable1, vernerTable2,
			fluorescenceTable, abundances, id, numOfPhotons, n_e,
			agnformula, initSpectrumDirFilter),
		m_structureModel{ static_cast<const PAGNSmoothStructureModelB&>(*structureModel) }
	{
	}

	void PAGNSmoothSimulationTeam::run(std::string_view pathToStorageFolder)
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
					// is inside the smooth agn structure?
					// yes: => calculate the distance to boundary
					// no : => will enter?
					//         yes: => move the photon to the entering point && continue
					//         no : => register the photon
					phys_float distanceToBoundary{ 0 };
					if (m_structureModel.isInside(photon.position()))
					{
						auto optionalDistanceToBoundary = m_structureModel.distanceToBoundary(photon.position(), photon.omega());

						if (optionalDistanceToBoundary.has_value())
						{
							distanceToBoundary = optionalDistanceToBoundary.value();
						}
						else
						{
							m_simMng.registerPhoton(photon);
							break;
						}
					}
					else
					{
						auto optionalDistanceToEnteringPoint{ m_structureModel.distanceToEnteringPoint(photon.position(),
																									photon.omega()) };

						// if there is a distance value ie the photon will enter the structure
						if (optionalDistanceToEnteringPoint.has_value())
						{
							// we add 100 cm in order to be sure that the photon is inside the geometry
							photon.translate(optionalDistanceToEnteringPoint.value() + 100.0_cm);

							// now we (want to) take the photon out of the agn structure

							// "double check" if the photon is effectively out,
							// we do this because we dont have exact precision
							// with some geometric forms in physapi, like tori,
							// therefore it can cause unwanted "loops", and
							// we want to avoid those loops. thus if the photon
							// is still inside the agn internal structure we
							// simply register it and "move" to the next photon
							if (!m_structureModel.isInside(photon.position()))
							{
								m_simMng.registerPhoton(photon);
								break;
							}
							// if the photon is now inside the agn internal structure
							// we go the the begining of the while loop
							else
							{
								continue;
							}
						}
						// if the photon will no enter the agn structure
						// we register it and "move" to the next photon
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
							m_structureModel.distanceToInteractionPoint(m_lambdaMean,
																		m_escapeProbability,
																		m_randomValuesMng.random_0_1()) };
						photon.translate(distanceToInteractionPoint);
						m_simMng.photonInteracted(true);
					}
					else
					{
						photon.translate(distanceToBoundary + 100.0_cm);

						m_simMng.enteredGeometry(false);
						continue;
					}
				}
			}
		}
	}

	std::optional<phys_float> PAGNSmoothSimulationTeam::distanceToBoundary(PSimplePhoton& photon)
	{
		// is inside the smooth agn structure?
		// yes: => calculate the distance to boundary
		// no : => will enter?
		//         yes: => move the photon to the entering point && return the distande to boundary
		//         no : => return no distance to boundary
		if (m_structureModel.isInside(photon.position()))
		{
			return  m_structureModel.distanceToBoundary(photon.position(), photon.omega());
		}
		else
		{
			auto optionalDistanceToEnteringPoint{ m_structureModel.distanceToEnteringPoint(photon.position(),
																									photon.omega()) };

			// if there is a distance value ie the photon will enter the structure
			if (optionalDistanceToEnteringPoint.has_value())
			{
				// we add 100 cm in order to be sure that the photon is inside the geometry
				photon.translate(optionalDistanceToEnteringPoint.value() + 100.0_cm);

				return  m_structureModel.distanceToBoundary(photon.position(), photon.omega());
			}
			else
			{
				return std::nullopt;
			}
		}
	}
}// namespace agn