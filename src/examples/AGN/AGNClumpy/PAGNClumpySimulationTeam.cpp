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
		phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
		const std::shared_ptr<PAGNFormula> agnformula,
		const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter)
		: PAGNSimulationTeamB(vernerTable1, vernerTable2,
			fluorescenceTable, abundances,
			id, numOfPhotons, n_e, T_e,
			agnformula, initSpectrumDirFilter),
		m_structureModel{ static_cast<const PAGNClumpyStructureModelB&>(*structureModel) },
		m_cloudIndex{ std::nullopt }, m_cloud(PPosition(), m_structureModel.cloudRadius()),
		m_cloudFinder{ m_structureModel.clouds(), m_structureModel.cloudRadius() }
	{
	}

	// move the photon inside the agn internal structure and get the distance to
	// closest external boundary
	std::optional<phys_float> PAGNClumpySimulationTeam::distanceToBoundary(PSimplePhoton& photon)
	{
		// is inside the internal agn structure(inside the last cloud)?
		// yes: => calculate the distance to boundary
		// no : => will enter other cloud?
		//         yes: => move the photon to the entering point && return the distande to boundary
		//         no : => return no distance to boundary

		if (m_cloudIndex.has_value())
		{
			m_cloud.setPosition(m_structureModel.clouds()[m_cloudIndex.value()]);
			if (m_cloud.isInside(photon.position()))
			{
				return m_cloud.distanceToBoundaryFromInside(photon.position(), photon.omega());
			}
		}

		m_cloudIndex = m_cloudFinder(photon);// find cloud for the photon => return index

		if (m_cloudIndex.has_value())
		{
			/*
			todo: move the photon inside the corresponding cloud and return the distande to boundary
			*/
			m_cloud.setPosition(m_structureModel.clouds()[m_cloudIndex.value()]);

			if (m_cloud.isInside(photon.position()))
			{
				return m_cloud.distanceToBoundaryFromInside(photon.position(), photon.omega());
			}
			else
			{
				auto distanceToEnteringPoint{ m_cloud.distanceToBoundaryFromOutside(photon.position(),photon.omega()) };
				photon.translate(distanceToEnteringPoint + 100.0_cm);
				auto distanceToBoundary_v{ m_cloud.distanceToBoundaryFromInside(photon.position(),photon.omega()) };

				//double check
				if (distanceToBoundary_v >= 0)
				{
					return distanceToBoundary_v;
				}
				else
				{
					return std::nullopt;
				}

			}
		}
		else
		{
			return std::nullopt;
		}

	}
}