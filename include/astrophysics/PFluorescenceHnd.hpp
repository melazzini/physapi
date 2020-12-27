#pragma once

#include "PFluorescenceTable.hpp"
#include <memory>
#include <optional>

namespace physapi
{
    /**
     * @brief Handles the simulation of fluorescence.
     * 
     * @ingroup astrophysics
     * 
     * **Example:**
     * 
     * @code{.cpp}
     * 
#include "PFluorescenceHnd.hpp"
#include <fstream>
#include <memory>

    using namespace std;
    using namespace physapi;

    int main()
    {
        // we need the abundances
        PAbundanceTable abundances;

        // for memory efficiency(special when working with multiple threads)
        // we need a shared pointer to the fluorescence table
        shared_ptr<PFluorescenceTable> table = make_shared<PFluorescenceTable>(abundances);

        // now we can create the fluorescence simulator
        PFluorescenceHnd fluorescenceHnd(table);

        // the next for loop demostrate how easy it is to use the fluorescence simulator
        // with 10 trials we run the simulation with the Fe possible lines

        for (size_t i = 0; i < 10; i++)
        {
            cout << "trial #" << i + 1 << ":" << endl;

            auto optionalLine = fluorescenceHnd.run(eZ::Fe, 1);

            if (optionalLine.has_value())
            {
                auto& line = optionalLine.value();

                cout << line.first.atomicNumber() << "  "
                    << line.first.ionizationState() << "  "
                    << line.first.shellNUmber() << "  "
                    << line.first.augerElectrons() << "  "
                    << line.first.lineNumber() << "  "
                    << line.second.lineEnergy() << "  "
                    << line.second.lineYield() << endl;
            }
            else
            {
                cout << "no fluorescence occurred" << endl;
            }
            cout << endl;
        }
    }
     * @endcode
     * 
     * output:
     * 
        trial #1:

        no fluorescence occurred

        trial #2:

        no fluorescence occurred

        trial #3:
        
        no fluorescence occurred

        trial #4:
        
        26  1  1  0  1  6391.5  0.1013

        trial #5:
        
        no fluorescence occurred

        trial #6:
        
        no fluorescence occurred

        trial #7:
        
        26  1  1  0  2  6404.7  0.2026

        trial #8:
        
        no fluorescence occurred

        trial #9:
        
        no fluorescence occurred

        trial #10:
        
        26  1  1  0  3  7056.7  0.0127
     * 
     */
    class PFluorescenceHnd
    {
    private:
        // fluorescence table
        const std::shared_ptr<PFluorescenceTable> m_fluorescenceTable;

        // random numbers manager
        PRandom m_randMng;

    public:
        /**
         * @brief Construct a new PFluorescenceHnd object
         * 
         */
        explicit PFluorescenceHnd(const std::shared_ptr<PFluorescenceTable> fluorescenceTable)
        :m_fluorescenceTable{fluorescenceTable}
        {}

        /**
         * @brief Destroy the PFluorescenceHnd object
         * 
         */
        virtual ~PFluorescenceHnd() {}

        /**
         * @brief Simulate fluorescence.
         * 
         * @param z atomic number
         * @param is shell number
         * @param st ionization state
         * @param aug auger electrons
         * @return std::optional<t_fluorescentLine> optional fluorescence line
         */
        t_fluorescentLine run(eZ z, phys_size is, phys_size st = 1, phys_size aug = 0);

    private:
        std::vector<phys_float> yieldList(const std::vector<t_fluorescentLine> &lines) const;
    };
} // namespace physapi
