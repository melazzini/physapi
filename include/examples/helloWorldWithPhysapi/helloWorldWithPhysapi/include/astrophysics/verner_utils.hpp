#pragma once
#include "utils.hpp"
#include <map>
#include "PAbundanceTable.hpp"
namespace physapi
{
    /*********************************************************************
    *                   Verner Table 1 (BEGIN)
    *********************************************************************/

    // NUMBER OF COLUMNS OF TABLE 1
    constexpr phys_size VERNERTABLE1_COLS = 10;

    // DESCRIPTION OF THE KEYS OF TABLE 1 @keystable1
    enum class eVernerTable1Key
    {
        Z = 0,  // ATOMIC NUMBER
        Ne = 1, // NUMBER OF ELECTRONS
        Is = 2  // SHELL NUMBER "n"
    };

    // DESCRIPTION OF THE VALUES OF TABLE 1 @valuestable1
    enum class eVernerTable1Val
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
    using _t_nz_ne_is = std::array<phys_size, 3>;

    // table1 key type
    using t_vernertable1Key = _t_nz_ne_is;

    // table1 value type
    //this type is an array to store the values of
    //the parameters of VernerTable1, used in the
    //fitting algorithm, ie not to store nz,ne,is,
    //but for example E_max, etc.
    using t_vernertable1Value = std::array<phys_float, 6>;

    const std::string DEFAULT_FILE_PATH_VERNERTABLE1 = "./include/astrophysics/tables/vernertable1.txt";

    class PVernerTable1 : public std::map<t_vernertable1Key, t_vernertable1Value>
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
        const PAbundanceTable& m_abundances;

    public:
        PVernerTable1(const PAbundanceTable& abundances) : m_abundances{ abundances }
        {
        }

        void loadElement(const std::array<phys_float, VERNERTABLE1_COLS>& row);
    };

    // correct the problem with the shell numbers
    class PVernerTable1Hnd
    {
        using t_vect = std::vector<std::pair<_t_nz_ne_is, t_vernertable1Value>>;

    public:
        void operator()(t_vect& v) const
        {
            formatShellNumber(v);
        }

    private:
        void formatShellNumber(t_vect& v) const;
        phys_size getShellNumber(phys_size is_) const;
    };

    class PLoadVernerTable1
    {
    public:
        std::pair<_t_nz_ne_is, t_vernertable1Value> operator()(
            const std::array<phys_float, VERNERTABLE1_COLS>& a)
        {
            return { {static_cast<phys_size>(a[0]),
                     static_cast<phys_size>(a[1]),
                     static_cast<phys_size>(a[2])},
                    {a[4], a[5], a[6], a[7], a[8], a[9]} };
        }
    };

    class PSelectorVernerTable1
    {
    public:
        template <typename T, typename T2>
        bool operator()(const T& a, const T2& abundances)
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
    *                   Verner Table 1 (END)
    *********************************************************************/

    /*********************************************************************
    *                   Verner Table 2 (BEGIN)
    *********************************************************************/
    //this type is an array to store the values of
        //  atomic number: nz
        //  number of electrons: ne
    using t_nz_ne = std::array<phys_size, 2>;

    // table1 key type
    using t_vernertable2Key = t_nz_ne;

    // table2 value type
    //this type is an array to store the values of
    //the parameters of VernerTable2, used in the
    //fitting algorithm
    using t_vernertable2Value = std::array<phys_float, 9>;

    // NUMBER OF COLUMNS OF TABLE 2
    constexpr phys_size VERNERTABLE2_COLS = 11;

    // DEFAULT PATH OF TABLE 2
    const std::string DEFAULT_FILE_PATH_VERNERTABLE2 = "./include/astrophysics/tables/vernertable2.txt";

    // DESCRIPTION OF THE KEYS OF TABLE 2 @keystable2
    enum class eVernerTable2Key
    {
        Z = 0,  // ATOMIC NUMBER
        Ne = 1, // NUMBER OF ELECTRONS
    };

    // DESCRIPTION OF THE VALUES OF TABLE 2 @valuestable2
    enum class eVernerTable2Val
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

    class PVernerTable2 : public std::map<t_vernertable2Key, t_vernertable2Value>
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
        const PAbundanceTable& m_abundances;

    public:
        PVernerTable2(const PAbundanceTable& abundances)
            : m_abundances{ abundances }
        {
        }

        void loadElement(const std::array<phys_float, VERNERTABLE2_COLS>& row);
    };

    /*********************************************************************
    *                   Verner Table 2 (END)
    *********************************************************************/

    /**
     * @brief This operator is needed to normalize the value
     * of the sigma cross section given by Prof. Verner's algorithm
     * 
     */
    constexpr phys_lfloat operator"" _Mb(phys_lfloat value)
    {
        return value * 1.E-18;
    }

    // DESCRIPTION OF AN ABSORBING ELEMENT
    // THE FISRT THREE ARE THE SAME AS 
    // VERNERTABLE1 KEY PROPERTIES(Z,NE,IS)
    enum class eAbsorbingElement
    {
        Z = static_cast<phys_size>(eVernerTable1Key::Z),  // ATOMIC NUMBER
        Ne = static_cast<phys_size>(eVernerTable1Key::Ne), // NUMBER OF ELECTRONS
        Is = static_cast<phys_size>(eVernerTable1Key::Is),  // SHELL NUMBER "n"
        SIGMA = 3 // PHOTOIONIZATION CROSS SECTION OF THE GIVEN ELEMENT
    };

    // NUMBER OF PROPERTIES THAT DETERMINE AN ABSORBING ELEMENT (SEE @eAbsorbingElement)
    constexpr phys_size ABSORBINGELEMENT_PROPERTIES = 4;

} // namespace physapi
