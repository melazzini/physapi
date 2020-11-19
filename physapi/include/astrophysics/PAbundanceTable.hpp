#pragma once

#include "astrophysics_utils.hpp"
#include <map>
#include <iostream>
#include <string>

namespace physapi
{
    /**
     * @brief This class represents a table of abundances.
     * 
     * Abundance values are relative to the Hydrogen atom.
     * 
     * @ingroup astrophysics
     * 
     */
    class PAbundanceTable
    {
        static const std::string DEFAULT_FILE_PATH;

        // ABUNDANCE TABLE(file) DESCRIPTION
        enum eCOLS
        {
            Z = 0,    // ATOMIC NUMBER COLUMN IN THE ABUNDANCE TABLE
            VALUE = 1 // ABUNDANCE VALUE IN THE ABUNDANCE TABLE
        };

    private:
        // raw table of abundances
        std::map<eZ, phys_float> m_abundances;

        // abundances table
        std::string m_file;

    public:
        /**
         * @brief Construct a new PAbundanceTable object
         * 
         * @param file abundances table
         */
        explicit PAbundanceTable(const std::string &file = DEFAULT_FILE_PATH)
            : m_file{file}
        {
            loadFromFile<ABUNDANCE_TABLE_COLS>(m_file, *this);
        }

        /**
         * @brief Destroy the PAbundanceTable object
         * 
         */
        virtual ~PAbundanceTable() {}

        /**
         * @brief Get the abundance for the corresponding atomic number.
         * 
         * 
         * @param atomicNumber atomic number(Z)
         * @return phys_float abundance
         * @return 0 if no element
         */
        phys_float abundance(eZ atomicNumber) const;

        /**
         * @brief get \f$ \sum_{i} (z_{i}*a_{i}) \f$
         * 
         * @return phys_float 
         */
        phys_float total() const;

        template <phys_size columns, typename Target, typename T>
        friend void loadFromFile(const std::string &file, Target &target, phys_size maxNunOfRows);

        friend std::ostream &operator<<(std::ostream &os, const PAbundanceTable &table);

    private:
        // we use this to load elements into the table
        void loadElement(const std::array<phys_float, ABUNDANCE_TABLE_COLS> &row);
    };
} // namespace physapi
