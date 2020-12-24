#include"PAGNSimulationMng.hpp"

namespace agn
{
	PAGNSimulationMng::PAGNSimulationMng()
		:m_isGone{false},m_isInside{false},
		m_currentLine{t_fluorescentLine()},
		m_currentPhotonType{eTypeOfAGNPhoton::INTRINSIC},
		m_currentPhotonIndex{}
	{
	}
	PAGNSimulationMng::~PAGNSimulationMng()
	{
		printRemainder();
		m_fout.close();
	}
	void PAGNSimulationMng::setFileName(std::string_view fileName)
	{
		m_file = fileName;
	}
	void PAGNSimulationMng::registerPhoton(const PSimplePhoton& photon)
	{
        t_dataToBeStored newData;
        newData[static_cast<phys_size>(eAGNData::ENERGY)] = photon.energy();
        newData[static_cast<phys_size>(eAGNData::THETA)] = photon.omega().theta();
        newData[static_cast<phys_size>(eAGNData::PHY)] = photon.omega().phi();
        newData[static_cast<phys_size>(eAGNData::TYPE)] = static_cast<phys_float>(m_currentPhotonType);

        auto lineKey = m_currentLine.first;

        // we put fekalpha first because it is the most frequently encountered line
        // thus, we wont expend looking for other lines(for time efficiency)
        if (lineKey == FeKalpha1 || lineKey == FeKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::FeKalpha);
        }
        else if (lineKey == CKalpha1 || lineKey == CKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::CKalpha);
        }
        else if (lineKey == NKalpha1 || lineKey == NKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::NKalpha);
        }
        else if (lineKey == OKalpha1 || lineKey == OKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::OKalpha);
        }
        else if (lineKey == NeKalpha1 || lineKey == NeKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::NeKalpha);
        }
        else if (lineKey == NaKalpha1 || lineKey == NaKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::NaKalpha);
        }
        else if (lineKey == MgKalpha1 || lineKey == MgKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::MgKalpha);
        }
        else if (lineKey == AlKalpha1 || lineKey == AlKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::AlKalpha);
        }
        else if (lineKey == SiKalpha1 || lineKey == SiKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::SiKalpha);
        }
        else if (lineKey == SKalpha1 || lineKey == SKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::SKalpha);
        }
        else if (lineKey == ArKalpha1 || lineKey == ArKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::ArKalpha);
        }
        else if (lineKey == CaKalpha1 || lineKey == CaKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::CaKalpha);
        }
        else if (lineKey == CrKalpha1 || lineKey == CrKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::CrKalpha);
        }
        else if (lineKey == NiKalpha1 || lineKey == NiKalpha2)
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::NiKalpha);
        }
        else
        {
            newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::NONE);
        }

        m_dataToBeStored[m_currentPhotonIndex] = newData;

        if (m_currentPhotonIndex + 1 >= m_dataToBeStored.size())
        {
            m_currentPhotonIndex = 0;
            printData();
        }
        else
        {
            m_currentPhotonIndex++;
        }

        reset();
	}
	void PAGNSimulationMng::reset()
	{
		m_isGone = false;
		m_currentLine = t_fluorescentLine();
		m_currentPhotonType = eTypeOfAGNPhoton::INTRINSIC;
		m_isInside = false;
	}
	void PAGNSimulationMng::printData()
	{
		std::string data, energy, theta, phi, typeOfPhoton, typeOfLine;
		for (auto&& photon_data_row_i : m_dataToBeStored)
		{
			energy = std::to_string(photon_data_row_i[static_cast<phys_size>(eAGNData::ENERGY)] / 1.0_eV);
			if (photon_data_row_i[static_cast<phys_size>(eAGNData::ENERGY)] / 1.0_eV < 90.0)
			{
				std::cout << std::endl;
				std::cout << "bad!!!" << std::endl;
				abort();
			}

			theta = std::to_string(photon_data_row_i[static_cast<phys_size>(eAGNData::THETA)]);
			phi = std::to_string(photon_data_row_i[static_cast<phys_size>(eAGNData::PHY)]);
			typeOfPhoton = std::to_string((phys_size)photon_data_row_i[static_cast<phys_size>(eAGNData::TYPE)]);
			typeOfLine = std::to_string((phys_size)photon_data_row_i[static_cast<phys_size>(eAGNData::LINE)]);

			data += (energy + " " + theta + " " + phi + " " + typeOfPhoton + " " + typeOfLine + "\n");
		}

		m_fout << data;
	}
	void PAGNSimulationMng::printRemainder()
	{
		std::string data, energy, theta, phi, typeOfPhoton, typeOfLine;

		for (phys_size i = 0; i < m_currentPhotonIndex; i++)
		{
			auto photon_data_row_i = m_dataToBeStored[i];

			energy = std::to_string(photon_data_row_i[static_cast<phys_size>(eAGNData::ENERGY)] / 1.0_eV);
			theta = std::to_string(photon_data_row_i[static_cast<phys_size>(eAGNData::THETA)]);
			phi = std::to_string(photon_data_row_i[static_cast<phys_size>(eAGNData::PHY)]);
			typeOfPhoton = std::to_string((phys_size)photon_data_row_i[static_cast<phys_size>(eAGNData::TYPE)]);
			typeOfLine = std::to_string((phys_size)photon_data_row_i[static_cast<phys_size>(eAGNData::LINE)]);

			data += (energy + " " + theta + " " + phi + " " + typeOfPhoton + " " + typeOfLine + "\n");
		}

		m_fout << data;
	}
}// namespace agn