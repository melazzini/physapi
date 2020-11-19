#pragma once

#include "PFluorescenceTable.hpp"

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
        #include "PFluorescenceHnd.hpp"
        #include <fstream>

        using namespace std;
        using namespace physapi;

        int main()
        {
            PFluorescenceHnd fluorescenceHnd;

            for (size_t i = 0; i < 10; i++)
            {
                cout << "trial #" << i + 1 << ":" << endl;

                auto line = fluorescenceHnd.run(eZ::Fe, 1);

                if (line == t_fluorescentLine())
                {
                    cout << "no fluorescence occurred" << endl;
                }
                else
                {
                    cout << line.first.atomicNumber() << "  "
                         << line.first.ionizationState() << "  "
                         << line.first.shellNUmber() << "  "
                         << line.first.augerElectrons() << "  "
                         << line.first.lineNumber() << "  "
                         << line.second.lineEnergy() << "  "
                         << line.second.lineYield() << endl;
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
        PFluorescenceTable m_fluorescenceTable;

        // random numbers manager
        PRandom m_randMng;

    public:
        /**
         * @brief Construct a new PFluorescenceHnd object
         * 
         */
        explicit PFluorescenceHnd() {}

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
         * @return t_fluorescentLine fluorescence line
         * @return t_fluorescentLine() if no fluorescence happened 
         */
        t_fluorescentLine run(eZ z, phys_size is, phys_size st = 1, phys_size aug = 0);

    private:
        std::vector<phys_float> yieldList(const std::vector<t_fluorescentLine> &lines) const;
    };
} // namespace physapi
