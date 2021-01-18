#pragma once

#include "astrophysics_utils.hpp"
#include <map>
#include<memory>
#include "PAbundanceTable.hpp"

namespace physapi
{
    /**
     * @brief This class represents a fluorescence table.
     * 
     * @ingroup astrophysics
     * 
     * **Example**
     * 
     * @code{.cpp}
        #include "PFluorescenceTable.hpp"
        #include <fstream>

        using namespace std;
        using namespace physapi;

        int main()
        {
            PFluorescenceTable table;

            auto lines = table.lines(eZ::Fe, 1, 1, 0);

            if (lines.empty())
            {
                cout << "there is no line for:    "
                    << "z=1, st=1, is=1, aug=0" << endl;
            }
            else
            {
                for (auto &&line : lines)
                {
                    cout << line.first[0] << "  "
                        << line.first[1] << "  "
                        << line.first[2] << "  "
                        << line.first[3] << "  "
                        << line.first[4] << "  "
                        << line.second.lineEnergy() << "  "
                        << line.second.lineYield() << endl;
                }
            }
        }
     * @endcode
     * 
     * output:
     * 
        26  1  1  0  1  6391.5  0.1013

        26  1  1  0  2  6404.7  0.2026
        
        26  1  1  0  3  7056.7  0.0127
        
        26  1  1  0  4  7058.3  0.0254
     * 
     * 
     */
    class PFluorescenceTable
    {
        // default file path
        static const std::string DEFAULT_FILE_PATH;

        // description of the fluorescence table
        enum eFTable
        {
            Z = 0,    // ATOMIC NUMBER
            ST = 1,   // IONIZATION STATE
            IS = 2,   // SHELL NUMBER
            AUG = 3,  // NUMBER OF AUGER ELECTRONS
            LINE = 4, // TRANSITION LINE
            E = 5,    // ENERGY
            W = 6     // YIELD
        };

    private:
        // data
        std::map<PFluorescenceKey, PFluorescenceValue> m_fluorescenceTable;

        // fluorescence table file
        std::string m_file;

        // abundances
        const PAbundanceTable& m_abundances;

    public:
        /**
         * @brief Construct a new PFluorescenceTable object
         * 
         * @param file fluorescence table file path
         */
        PFluorescenceTable(const PAbundanceTable& abundances, const std::string file = DEFAULT_FILE_PATH)
            : m_file{file}, m_abundances{abundances}
        {
            loadFromFile<FLUORESCENCE_TABLE_COLS>(m_file, *this);
        }

        /**
         * @brief Destroy the PFluorescenceTable object
         * 
         */
        virtual ~PFluorescenceTable() {}

        /**
         * @brief Get the fluorescence line corresponding to the given key
         * 
         * @param lineKey key of the fluorescence line
         * @return t_fluorescentLine fluorescence line(key + value)
         */
        t_fluorescentLine line(const PFluorescenceKey &lineKey) const;

        /**
         * @brief Get a list(vector) of all lines corresponding to z, st, is and aug
         * 
         * @param z atomic number
         * @param ionizationState st 
         * @param shell is
         * @param augerElectrons aug
         * @return std::vector<t_fluorescentLine> 
         */
        std::vector<t_fluorescentLine> lines(eZ z,
                                             phys_size ionizationState,
                                             phys_size shell,
                                             phys_size augerElectrons) const;

        template <phys_size columns, typename Target, typename T>
        friend void loadFromFile(const std::string &file, Target &target, phys_size maxNunOfRows);

    private:
        void loadElement(const std::array<phys_float, FLUORESCENCE_TABLE_COLS> &row);
    };
} // namespace physapi
