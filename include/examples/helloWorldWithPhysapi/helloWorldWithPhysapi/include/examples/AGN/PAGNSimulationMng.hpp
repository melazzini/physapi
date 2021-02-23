#pragma once
#include"agn_utils.hpp"

namespace agn
{
	class PAGNSimulationMng
	{
		// STORED DATA TYPE(TYPE OF EACH ROW)
		using t_dataToBeStored = std::array<phys_float, AGN_DATA_COLS>;

		// MAXIMUM NUMBER OF PHOTONS TO BE STORED AT ONCE
		static constexpr phys_size MAX_NUM_OF_PHOTONS = 10000;

	public:
		explicit PAGNSimulationMng();

		virtual ~PAGNSimulationMng();

		void setFileName(std::string_view fileName);

		// returns true if the photon escaped the torus
			// or was absorbed, else false
		phys_bool isGone() const { return m_isGone; }

		// register the photon
		void registerPhoton(const PSimplePhoton& photon);

		void setFluorescenceLine(const t_fluorescentLine& line)
		{
			m_currentLine = line;
		}

		void photonHasBeenAbsorbed(phys_bool trueOrFalse)
		{
			m_isGone = trueOrFalse;
		}

		void photonInteracted(phys_bool trueOrFalse=true)
		{
			if (trueOrFalse)
			{
				m_currentPhotonType = eTypeOfAGNPhoton::REFLECTEDONLY;
			}
		}

		void reset();

		void printData();

		void printRemainder();

		// return true if the photon is inside the geometry
		phys_bool isInside() const { return m_isInside; }

		void enteredGeometry(phys_bool yesNo);

	private:
		// the photon escaped the torus or was absorbed
		phys_bool m_isGone;

		phys_bool m_isInside;

		phys_bool m_enteredGeometryAtleastOnce;

		// this line has produced the photon current photon
		t_fluorescentLine m_currentLine;

		// std::array<t_dataToBeStored, MAX_NUM_OF_PHOTONS> m_dataToBeStored;
		std::vector<t_dataToBeStored> m_dataToBeStored;

		// type of the current photon
		eTypeOfAGNPhoton m_currentPhotonType;

		phys_size m_currentPhotonIndex;

		std::string_view m_file;

		std::ofstream m_fout; // stream
	};
}// namespace agn