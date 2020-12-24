#include"PAGNClumpySimulationTeam.hpp"
#include"PAGNClumpyStructureModelB.hpp"
#include"PAGNInitSpectrumDirectionFilter.hpp"
namespace agn
{
	PAGNClumpySimulationTeam::PAGNClumpySimulationTeam(
		const std::shared_ptr<PAGNStructureModelB> structureModel,
		const std::shared_ptr<PVernerTable1> vernerTable1,
		const std::shared_ptr<PVernerTable2> vernerTable2,
		const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
		const std::shared_ptr<PAbundanceTable> abundances,
		phys_size id, phys_float numOfPhotons, phys_float n_e,
		const std::shared_ptr<PAGNFormula> agnformula,
		const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter)
		: PAGNSimulationTeamB(vernerTable1, vernerTable2,
			fluorescenceTable, abundances,
			id, numOfPhotons, n_e,
			agnformula, initSpectrumDirFilter),
		m_structureModel{ static_cast<const PAGNClumpyStructureModelB&>(*structureModel) },
		m_cloudIndex{std::nullopt}, m_cloud(PPosition(),m_structureModel.cloudRadius())
	{
	}

	void PAGNClumpySimulationTeam::run(std::string_view pathToStorageFolder)
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

				//1. We perform the simulation only if the photon is inside the agn
				while (!m_simMng.isGone())
				{
					//2. We find the closest clump on the way and if no clump was
					//found then we break the simulation and register the photon
					//(see below how we register the photon). It means that the
					//next steps are performed only when we have found a clump on
					//the photon's way. We perform this step only if the photon
					//is not inside any cloud
					if (!m_simMng.isInside())
					{
						//if the photon will intersect some cloud then
						//we get the index of that cloud and the length
						//of the trajectory that intersects that cloud
						m_cloudIndex;// find cloud for the photon => return index
						
						if (m_cloudIndex.has_value())
						{
							/*
							todo: move the photon inside the corresponding cloud, tell the sim mng and restart the while loop
							*/
						}
						else
						{
							m_simMng.registerPhoton(photon);
							break;
						}
					}

					// if we get to this point ie the photon is inside a cloud
					m_cloud.setPosition(m_structureModel.clouds()[m_cloudIndex.value()]);

					phys_float distanceToBoundary{ m_cloud.distanceToBoundaryFromInside(photon.position(),photon.omega())};

					if (distanceToBoundary < 0)
					{
						std::cout << "distance to boundary cannot be less that zero!" << std::endl;
						abort();
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
	std::optional<phys_float> PAGNClumpySimulationTeam::distanceToBoundary(PSimplePhoton& photon)
	{
		return std::optional<phys_float>();
	}
}