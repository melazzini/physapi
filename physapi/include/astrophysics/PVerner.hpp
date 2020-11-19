#pragma once

#include "astrophysics_utils.hpp"
#include <map>
#include <iostream>
#include "PAbundanceTable.hpp"

namespace physapi
{
    /**
     * @brief This class is a functor for calculating the photonionization
     * cross section.
     * 
     * This class is based on  
     * <a href="https://ui.adsabs.harvard.edu/abs/1996ApJ...465..487V/abstract"> Verner </a>.
     * 
     * @ingroup astrophysics
     * 
     * **Example:**
     * 
     * @code{.cpp}
        #include "PVerner.hpp"
        #include <fstream>

        using namespace std;
        using namespace physapi;

        int main()
        {
            PVerner verner;

            ofstream fout;

            fout.open("photoionization.txt");

            for (phys_float energy_i = 1.0_eV; energy_i < 300.0E3_eV; energy_i += 1.0_eV)
            {
                fout << energy_i / 1.0_eV << "    " << verner(energy_i) << endl;
            }
        }
     * @endcode
     * 
     * 
     * output:
     * 
     * 
     * <a href="photoionization.txt"> photonionization.txt </a>
     * 
     * @image html photoionization.png
     * 
     */
    class PVerner
    {

        /*********************************************************************
         *                   Verner Table 1
         *********************************************************************/

        // NUMBER OF COLUMNS OF TABLE 1
        static constexpr phys_size TABLE1_COLS = 10;

        // DESCRIPTION OF THE KEYS OF TABLE 1 @keystable1
        enum class eTable1Key
        {
            Z = 0,  // ATOMIC NUMBER
            Ne = 1, // NUMBER OF ELECTRONS
            Is = 2  // SHELL NUMBER "n"
        };

        // DESCRIPTION OF THE VALUES OF TABLE 1 @valuestable1
        enum class eTable1Val
        {
            ETh = 0, // SUBSHELL IONIZATION THRESHOLD ENERGY
            E0,      // FIT PARAMETER
            SIGMA0,  // FIT PARAMETER
            Ya,      // FIT PARAMETER
            P,       // FIT PARAMETER
            Yw = 5   // FIT PARAMETER
        };

        //this type is an array to store the values of
        //  atomic number: nz
        //  number of electrons: ne
        //  shell number: is
        using t_nz_ne_is = std::array<phys_size, 3>;

        // table1 key type
        using t_table1Key = t_nz_ne_is;

        // table1 value type
        //this type is an array to store the values of
        //the parameters of VernerTable1, used in the
        //fitting algorithm, ie not to store nz,ne,is,
        //but for example E_max, etc.
        using t_table1Value = std::array<phys_float, 6>;

        static const std::string DEFAULT_FILE_PATH_TABLE1;

        class Table1 : public std::map<t_table1Key, t_table1Value>
        {
            // DESCRIPTION OF THE COLUMNS OF VERNER TABLE1
            enum eColTab1
            {
                Z = 0,      // ATOMIC NUMBER
                Ne = 1,     // NUMBER OF ELECTRONS
                Is = 2,     // SHELL NUMBER "n"
                L = 3,      // SUBSHELL (ORBITAL) QUANTUM NUMBER "l" = {s=0,p=1,d=2}
                Eth = 4,    // SUBSHELL IONIZATION THRESHOLD ENERGY
                E0 = 5,     // FIT PARAMETER
                SIGMA0 = 6, // FIT PARAMETER
                Ya = 7,     // FIT PARAMETER
                P = 8,      // FIT PARAMETER
                Yw = 9      // FIT PARAMETER
            };

        private:
            const PAbundanceTable &m_abundances;

        public:
            Table1(const PAbundanceTable &abundances) : m_abundances{abundances}
            {
            }

            void loadElement(const std::array<phys_float, TABLE1_COLS> &row)
            {
                auto z = static_cast<phys_size>(row[eColTab1::Z]);

                if (m_abundances.abundance(eZ(z)) == 0)
                {
                    return;
                }

                auto ne = static_cast<phys_size>(row[eColTab1::Ne]);

                if (z != ne)
                {
                    return;
                }

                auto is = static_cast<phys_size>(row[eColTab1::Is]);

                auto l = row[eColTab1::L]; // WE DO NOT USE THIS NUMBER!

                auto E_th = row[eColTab1::Eth];
                auto E_0 = row[eColTab1::E0];
                auto sigma_0 = row[eColTab1::SIGMA0];
                auto y_a = row[eColTab1::Ya];
                auto P = row[eColTab1::P];
                auto y_w = row[eColTab1::Yw];

                // static uint is_ = 0;

                if (find({z, ne, is}) != end())
                {
                    (*this)[{z, ne, is}] = {E_th, E_0, sigma_0, y_a, P, y_w};
                }
                else
                {
                    uint is_ = is;
                    while (find({z, ne, is}) != end())
                    {
                        is_++;
                    }

                    (*this)[{z, ne, is_}] = {E_th, E_0, sigma_0, y_a, P, y_w};
                }
            }
        };

        // correct the problem with the shell numbers
        class Table1Hnd
        {
            using t_vect = std::vector<std::pair<t_nz_ne_is, t_table1Value>>;

        public:
            void operator()(t_vect &v) const
            {
                formatShellNumber(v);
            }

        private:
            void formatShellNumber(t_vect &v) const;
            phys_size getShellNumber(phys_size is_) const;
        };

        class LoadTable1
        {
        public:
            std::pair<t_nz_ne_is, t_table1Value> operator()(
                const std::array<phys_float, TABLE1_COLS> &a)
            {
                return {{static_cast<phys_size>(a[0]),
                         static_cast<phys_size>(a[1]),
                         static_cast<phys_size>(a[2])},
                        {a[4], a[5], a[6], a[7], a[8], a[9]}};
            }
        };

        class SelectorTable1
        {
        public:
            template <typename T, typename T2>
            bool operator()(const T &a, const T2 &abundances)
            {
                //only if the element exists
                if (abundances.abundance(eZ(a.first[0])) != 0)
                {
                    //and if the element is neutral
                    return a.first[0] == a.first[1];
                }

                return false;
            }
        };

        /*********************************************************************
         *                   Verner Table 2
         *********************************************************************/

        //this type is an array to store the values of
        //  atomic number: nz
        //  number of electrons: ne
        using t_nz_ne = std::array<phys_size, 2>;

        // table1 key type
        using t_table2Key = t_nz_ne;

        // table2 value type
        //this type is an array to store the values of
        //the parameters of VernerTable2, used in the
        //fitting algorithm
        using t_table2Value = std::array<phys_float, 9>;

        // NUMBER OF COLUMNS OF TABLE 2
        static constexpr phys_size TABLE2_COLS = 11;

        // DEFAULT PATH OF TABLE 2
        static const std::string DEFAULT_FILE_PATH_TABLE2;

        // DESCRIPTION OF THE KEYS OF TABLE 2 @keystable2
        enum class eTable2Key
        {
            Z = 0,  // ATOMIC NUMBER
            Ne = 1, // NUMBER OF ELECTRONS
        };

        // DESCRIPTION OF THE VALUES OF TABLE 2 @valuestable2
        enum class eTable2Val
        {
            Eth = 0,    // SUBSHELL IONIZATION THRESHOLD ENERGY
            Emax = 1,   // IONIZATION ENERGY OF THE SUBSEQUENT INNER SHELL
            E0 = 2,     // FIT PARAMETER
            SIGMA0 = 3, // FIT PARAMETER
            Ya = 4,     // FIT PARAMETER
            P = 5,      // FIT PARAMETER
            Yw = 6,     // FIT PARAMETER
            Y0 = 7,     // FIT PARAMETER
            Y1 = 8      // FIT PARAMETER
        };

        class Table2 : public std::map<t_table2Key, t_table2Value>
        {
            // DESCRIPTION OF THE COLUMNS OF VERNER TABLE2
            enum eColTab2
            {
                Z = 0,      // ATOMIC NUMBER
                Ne = 1,     // NUMBER OF ELECTRONS
                Eth = 2,    // SUBSHELL IONIZATION THRESHOLD ENERGY
                Emax = 3,   // IONIZATION ENERGY OF THE SUBSEQUENT INNER SHELL
                E0 = 4,     // FIT PARAMETER
                SIGMA0 = 5, // FIT PARAMETER
                Ya = 6,     // FIT PARAMETER
                P = 7,      // FIT PARAMETER
                Yw = 8,     // FIT PARAMETER
                Y0 = 9,     // FIT PARAMETER
                Y1 = 10     // FIT PARAMETER
            };

        private:
            const PAbundanceTable &m_abundances;

        public:
            Table2(const PAbundanceTable &abundances)
                : m_abundances{abundances}
            {
            }

            void loadElement(const std::array<phys_float, TABLE2_COLS> &row)
            {
                auto z = static_cast<phys_size>(row[eColTab2::Z]);

                if (m_abundances.abundance(eZ(z)) == 0)
                {
                    return;
                }

                auto ne = static_cast<phys_size>(row[eColTab2::Ne]);

                if (z != ne)
                {
                    return;
                }

                auto E_th = row[eColTab2::Eth];
                auto E_max = row[eColTab2::Emax];
                auto E_0 = row[eColTab2::E0];
                auto sigma_0 = row[eColTab2::SIGMA0];
                auto y_a = row[eColTab2::Ya];
                auto P = row[eColTab2::P];
                auto y_w = row[eColTab2::Yw];
                auto y_0 = row[eColTab2::Y0];
                auto y_1 = row[eColTab2::Y1];

                (*this)[{z, ne}] = {E_th, E_max, E_0, sigma_0, y_a, P, y_w, y_0, y_1};
            }
        };

    private:
        // abundance table
        PAbundanceTable m_abundances;

        // path to table 1
        std::string m_table1File;

        // path to table 2
        std::string m_table2File;

        // table 1 see @keystable1 @valuestable1
        Table1 m_table1;

        // table 1 see @keystable2 @valuestable2
        Table2 m_table2;

        //this variable holds the value
        //of the photoionization cross
        //section
        phys_float m_sigma;

    public:
        /**
         * @brief Construct a new PVerner object
         * 
         * @param table1 path to table 1
         * @param table2 path to table 2
         */
        explicit PVerner(const std::string &table1 = DEFAULT_FILE_PATH_TABLE1,
                         const std::string &table2 = DEFAULT_FILE_PATH_TABLE2)
            : m_abundances{},
              m_table1File{table1},
              m_table2File{table2},
              m_table1{m_abundances},
              m_table2{m_abundances}
        {
            Table1Hnd config;
            PProcessTable<t_nz_ne_is, t_table1Value, TABLE1_COLS, LoadTable1> handler1{m_table1File, config};

            SelectorTable1 f;

            handler1(m_table1, f, m_abundances);

            // loadFromFile<TABLE1_COLS>(m_table1File, m_table1);

            loadFromFile<TABLE2_COLS>(m_table2File, m_table2);
        }

        /**
         * @brief Destroy the PVerner object
         * 
         */
        virtual ~PVerner() {}

        /**
         * @brief Get the photonionization cross section
         * 
         * @param energy photon energy
         * @param absorbingElements list of all absorbing elements for that energy
         * @return phys_float photonionization cross section
         */
        phys_float operator()(phys_float energy,
                              std::vector<std::array<phys_float, 4>> &absorbingElements);

        phys_float operator()(phys_float energy);

    private:
        phys_float getSigma(const Table1::const_iterator &it_Table1, phys_float e);

    private:
        //other internally required parameters:
        //required variables to implement the formula,
        //used in the algorithm, these variables
        //only hold temporary values in the calculation

        phys_float p1{};
        phys_float y{};
        phys_float q{};
        phys_float a{};
        phys_float b{};
        phys_float sigma{};
        phys_float x{};
        phys_float z{};

        //these symbols are used in
        //Prof Verner's algorithm,
        //the symbols denote current
        //atomic number, number
        //of electrons and shell
        //number
        phys_size nz, ne, is;

        //we need also for our implementation
        //a helper array to select from m_Table2
        //the current element
        t_nz_ne CurrentElementTable2;

        //out most-shell number
        phys_size nout{};

        //closest internal-shell number
        phys_size nint{};

        //Ionization energy of the
        //closest internal shell
        phys_float einn{};

        //the next arrays are helper tables
        //of quantum numbers

        //number of the orbital quantum number
        //corresponding to the number of the shell
        const std::array<phys_size, 7> l{
            0, 0, 1, 0, 1, 2, 0};

        //closest inner-shell corresponding to the number
        //of electrons
        const std::array<phys_size, 30> ninn{
            0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3,
            3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

        //outermost shell of an atom corresponding to the number
        //of electrons
        const std::array<phys_size, 30> ntot{
            1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4,
            5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7};
    };
} // namespace physapi
