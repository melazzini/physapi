#include "PFluorescenceTable.hpp"

namespace physapi
{
    const std::string PFluorescenceTable::DEFAULT_FILE_PATH = "./physapi/include/astrophysics/tables/fluorescencetable.txt";

    void PFluorescenceTable::loadElement(const std::array<phys_float, FLUORESCENCE_TABLE_COLS> &row)
    {
        phys_size z = row[eFTable::Z];
        // if the element has zero abundance then return
        if (!m_abundances.abundance(eZ(z)))
        {
            return;
        }

        //we dont work with weak fluorescence lines
        //and with lines,which aren't on the ground state
        //(see Kaastra p 447), for us the elements 1
        //and 4 in the array are: st and electron-energy
        //respectively, whereas in the table they
        //correspond to the second and fifth columns
        //notice that all electron energies start from
        //1eV
        phys_size st = row[eFTable::ST];
        phys_size line_ = row[eFTable::LINE];
        if (st > 1 || line_ > 4)
        {
            return;
        }

        phys_size aug = row[eFTable::AUG];
        phys_size is = row[eFTable::IS];

        m_fluorescenceTable[{eZ(z), st, is, aug, line_}] = {row[eFTable::E], row[eFTable::W]};
    }

    t_fluorescentLine PFluorescenceTable::line(const PFluorescenceKey &lineKey) const
    {
        if (m_fluorescenceTable.find(lineKey) != m_fluorescenceTable.end())
        {
            return {lineKey, m_fluorescenceTable.at(lineKey)};
        }
        else
        {
            return {};
        }
    }

    std::vector<t_fluorescentLine> PFluorescenceTable::lines(eZ z,
                                                             phys_size ionizationState,
                                                             phys_size shell,
                                                             phys_size augerElectrons) const
    {
        // result
        std::vector<t_fluorescentLine> lines_{};

        // lines begin at 1
        phys_size line_ = 1;

        // define the element key
        PFluorescenceKey key{z, ionizationState, shell, augerElectrons, line_};

        // we look for all the lines of the given element
        while (m_fluorescenceTable.find(key) != m_fluorescenceTable.end())
        {
            // we set that element to the list
            lines_.push_back({key, m_fluorescenceTable.at(key)});

            // we move forward looking for the next line
            line_++;
            key[4] = line_;
        }

        return lines_;
    }
} // namespace physapi
