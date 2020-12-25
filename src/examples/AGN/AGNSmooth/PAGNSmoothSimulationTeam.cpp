#include"PAGNSmoothSimulationTeam.hpp"

namespace agn
{
	PAGNSmoothSimulationTeam::PAGNSmoothSimulationTeam(
		const std::shared_ptr<PAGNStructureModelB> structureModel,
		const std::shared_ptr<PVernerTable1> vernerTable1,
		const std::shared_ptr<PVernerTable2> vernerTable2,
		const std::shared_ptr<PFluorescenceTable> fluorescenceTable,
		const std::shared_ptr<PAbundanceTable> abundances,
		phys_size id, phys_float numOfPhotons, phys_float n_e, phys_float T_e,
		const std::shared_ptr<PAGNFormula> agnformula,
		const std::shared_ptr<PAGNInitSpectrumDirectionFilter> initSpectrumDirFilter)
		: PAGNSimulationTeamB(vernerTable1, vernerTable2,
			fluorescenceTable, abundances, id, numOfPhotons, n_e, T_e,
			agnformula, initSpectrumDirFilter),
		m_structureModel{ static_cast<const PAGNSmoothStructureModelB&>(*structureModel) }
	{
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