#include "PAbundanceTable.hpp"
#include <iomanip>

namespace physapi
{
    const std::string PAbundanceTable::DEFAULT_FILE_PATH = "./include/astrophysics/tables/abundancetable.txt";

    void PAbundanceTable::loadElement(const std::array<phys_float, 3> &row)
    {
        m_abundances[static_cast<eZ>(row[eCOLS::Z])] = row[eCOLS::VALUE];
    }

    phys_float PAbundanceTable::abundance(eZ atomicNumber) const
    {
        if (m_abundances.find(atomicNumber) != m_abundances.end())
        {
            return m_abundances.at(atomicNumber);
        }

        return 0;
    }

    phys_float PAbundanceTable::total() const
    {
        phys_float sum{};

        for (auto &&element_i : m_abundances)
        {
            sum += (static_cast<phys_float>(element_i.first) * element_i.second);
        }

        return sum;
    }

    std::ostream &operator<<(std::ostream &os, const PAbundanceTable &table)
    {
        for (auto i = table.m_abundances.begin(); i != table.m_abundances.end(); i++)
        {
            os << static_cast<phys_size>(i->first) << " "
               << i->second;
            if (std::next(i) != table.m_abundances.end())
            {
                os << std::endl;
            }
        }

        return os;
    }

} // namespace physapi
