#pragma once

#include "agn_utils.hpp"
#include "AGNStructureModel.hpp"

namespace agn
{
    /**
     * @brief This class represents a group of objects that handle
     * the simulation of an agn spectrum photon by photon.
     * 
     * This class wraps up the overall algorithm of the simulation.
     * 
     * You can use this class in each thread when using multithreading
     * to simulate an agn spectrum.
     * 
     * A simulation team may consist of:
     * 
     *  * An initial spectrum maker
     *  * A fluorescence handler
     *  * A photonionization handler
     *  * A (monte carlo) compton scattering handler
     *  * A file saving handler
     *  * other internal objects
     * 
     * @ingroup agn 
     * 
     */
    class AGNSimulationTeam
    {
        // this internal class manages the simulation.
        // It controll that everything runs in the right
        // time in the code following each photon from
        // its creation until it gets registered or absorbed
        // it also register the photons
        class SimulationMng
        {

            // STORED DATA TYPE(TYPE OF EACH ROW)
            using t_dataToBeStored = std::array<phys_float, AGN_DATA_COLS>;

            // MAXIMUM NUMBER OF PHOTONS TO BE STORED AT ONCE
            static constexpr phys_size MAX_NUM_OF_PHOTONS = 10000;

        public:
            SimulationMng()
            {
                m_dataToBeStored.resize(MAX_NUM_OF_PHOTONS);
                m_currentPhotonIndex = 0;
            }

            ~SimulationMng()
            {
                printRemainder();
                m_fout.close();
            }

            void setFileName(const std::string &fileName)
            {
                m_file = fileName;

                if (m_file == "")
                {
                    std::cerr << "Simulation Manager: Invalid file name!" << std::endl;
                    std::abort();
                }

                m_fout.open(m_file);
            }

            // returns true if the photon escaped the torus
            // or was absorbed, else false
            phys_bool isGone() const { return m_isGone; }

            // register the photon
            void registerPhoton(const PSimplePhoton &photon)
            {
                t_dataToBeStored newData;
                newData[static_cast<phys_size>(eAGNData::ENERGY)] = photon.energy();
                newData[static_cast<phys_size>(eAGNData::THETA)] = photon.omega().theta();
                newData[static_cast<phys_size>(eAGNData::PHY)] = photon.omega().phi();
                newData[static_cast<phys_size>(eAGNData::TYPE)] = static_cast<phys_float>(m_currentPhotonType);

                auto lineKey = m_currentLine.first;

                if (lineKey == CKalpha1 || lineKey == CKalpha2)
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
                else if (lineKey == FeKalpha1 || lineKey == FeKalpha2)
                {
                    newData[static_cast<phys_size>(eAGNData::LINE)] = static_cast<phys_float>(eImportantFluorescentLines::FeKalpha);
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

            void setFluorescenceLine(const t_fluorescentLine &line)
            {
                m_currentLine = line;
            }

            void photonHasBeenAbsorbed(phys_bool trueOrFalse)
            {
                m_isGone = trueOrFalse;
            }

            void photonInteracted(phys_bool trueOrFalse)
            {
                if (trueOrFalse == true)
                {
                    m_currentPhotonType = eTypeOfAGNPhoton::REPROCESSED;
                }
            }

            void reset()
            {
                m_isGone = false;
                m_currentLine = t_fluorescentLine();
                m_currentPhotonType = eTypeOfAGNPhoton::INTRINSIC;
                m_isInside = false;
            }

            void printData()
            {
                std::string data, energy, theta, phi, typeOfPhoton, typeOfLine;
                for (auto &&photon_data_row_i : m_dataToBeStored)
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

            void printRemainder()
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

            // return true if the photon is inside the geometry
            phys_bool isInside() const { return m_isInside; }

            void enterGeometry(phys_bool yesNo) { m_isInside = yesNo; }

        private:
            // the photon escaped the torus or was absorbed
            phys_bool m_isGone = false;

            phys_bool m_isInside = false;

            // this line has produced the photon current photon
            t_fluorescentLine m_currentLine = t_fluorescentLine();

            // std::array<t_dataToBeStored, MAX_NUM_OF_PHOTONS> m_dataToBeStored;
            std::vector<t_dataToBeStored> m_dataToBeStored;

            // type of the current photon
            eTypeOfAGNPhoton m_currentPhotonType;

            phys_size m_currentPhotonIndex;

            std::string m_file;

            std::ofstream m_fout; // stream
        };

        // describes the list(vector) of absorbing elements
        enum eAbsorbingElementsDescription
        {
            Z = 0,    // element's atomic number
            Ne = 1,   // element's number of electrons
            IS = 2,   // element's shell number
            SIGMA = 3 // element's cross section
        };

    private:
        // instance id
        phys_size m_id;
        const AGNStructureModel &m_structureModel;

        // concentration of electrons
        phys_float m_n_e;

        // temperature of electrons
        phys_float m_T_e;

        // AGN INITIAL SPECTRUM FORMULA
        AGNFormula m_formula;

        // AGN INITIAL SPECTRUM MAKER
        PSpectrumMaker<AGNFormula::N_intervals, AGNFormula::SPECTRUM_PRECISION> m_spectrumMaker;

        // AGN INITIAL(INTRINSIC) SPECTRUM
        PSpectrum<AGNFormula::N_intervals> m_initSpectrum;

        // this is the fluorescence handler
        PFluorescenceHnd m_fluorescenceHnd;

        // this is the photonionization handler
        // it gives the photonionization cross section
        PVerner m_verner;

        // this handles compton simulation
        PComptonHnd m_comptonHnd;

        // filter the initial directions of the photons
        // created in the simulation
        DirectionFilter m_dirFilter;

        // random values generator
        PRandom m_randomValuesMng;

        // random directions generator
        PRandomDirection m_randDirMng;

        // simulation manager
        SimulationMng m_simMng;

    public:
        AGNSimulationTeam(const AGNStructureModel &structureModel,
                          phys_float n_e,
                          phys_float T_e,
                          phys_float numberOfPhotons,
                          phys_size id);

        /**
         * @brief run the simulation
         * 
         * @param pathToFolder 
         */
        void run(const std::string &pathToFolder);

    private:
        phys_float m_sigmaVerner; // photonionization cross section

        std::vector<std::array<phys_float, 4>> m_listOfAbsorbingElements;

        phys_float m_sigmaKleinNishina; // Klein Nishina cross section

        phys_float m_sigmaTotal; // total cross section

        phys_float m_lambdaMean; // mean free path

        phys_float m_escapeProbability;

    private:
        // get the mean free path for the given total cross section
        phys_float lambdaMean(phys_float sigmaTotal)
        {
            return (sigmaTotal > 0) ? (1.0 / (m_n_e * m_sigmaTotal)) : PLUS_INF;
        }

        // get the escape probability
        phys_float escapeProbability(phys_float distanceToBoundary, phys_float lambdaMean)
        {
            return (lambdaMean < PLUS_INF) ? std::exp(-distanceToBoundary / lambdaMean) : 1;
        }

        // get the fluorescent line, if any occurs, for the given arguments
        t_fluorescentLine fluorescentLine(const std::vector<std::array<phys_float, 4>> &absorbingElements,
                                          phys_float sigmaVerner);
    };

} // namespace agn
