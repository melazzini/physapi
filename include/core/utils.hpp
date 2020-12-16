#pragma once

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <random>

/**
 * @defgroup    core    Core
 * 
 * @brief This module handles the core structure of the api.
 * 
 * The core structure of the api consists of:
 * 
 *  * Fundamental data types and units.
 * 
 *  * Useful compile time functions.
 * 
 *  * Tools for handling file i/o and tables.
 * 
 *  * Threads handling tools.
 * 
 *  * Random number generation tools.
 *  
 *  * 3D and 2D vector tools.
 * 
 *  * GPU tools.
 * 
 *  * different physical constants.
 * 
 * @{
 * @}
 * 
 */

namespace physapi
{
    /**
     * @addtogroup  core
     * @{
     * 
     */

    /*****************************************************************
    *           FUNDAMENTAL DATA TYPES
    * *****************************************************************/

    /**
     * @brief Floating point type with minimum precision.
     * 
     * @note  If possible, use phys_float.
     * 
     */
    using phys_smallFloat = float;

    /**
     * @brief Floating point type with maximum presicion.
     * 
     * @note  If possible, use phys_float.
     * 
     * @see PHYS_LFLOAT_DIG
     * 
     */
    using phys_lfloat = long double;

    /**
     * @brief Standard floating point type.
     * 
     * @see    PHYS_FLOAT_DIG
     */
    using phys_float = double;

    /**
     * @brief Standard integer type.
     * 
     */
    using phys_int = int;

    /**
     * @brief Unsigned integer type.
     * 
     */
    using phys_uint = int;

    /**
     * @brief Long integer type.
     * 
     */
    using phys_lint = long int;

    /**
     * @brief Standard index type.
     * 
     * Use this type for counting elements.
     * 
     * @note  If you need a larger range, use phys_ull.
     * 
     */
    using phys_size = size_t;

    /**
     * @brief Index type with maximum size.
     * 
     * @note  If possible, use phys_size.
     * 
     */
    using phys_ull = unsigned long long;

    /**
     * @brief Standard boolean type. 
     * 
     */
    using phys_bool = bool;

    /*************************************************************************
    *                    SIZES OF OUR FUNDAMENTAL DATA TYPES 
    ************************************************************************/

    /**
     * @brief Digits of phys_float.
     * 
     * Number of decimal digits that can be rounded into a phys_float
     * and back without a change in the number of the decimal digits.
     * 
     */
    constexpr phys_size PHYS_FLOAT_DIG = DBL_DIG; // digits of phys_float

    /**
     * @brief Digits of phys_lfloat
     * 
     * Number of decimal digits that can be rounded into a phys_lfloat
     * and back without a change in the number of decimal digits.
     * 
     */
    constexpr phys_size PHYS_LFLOAT_DIG = LDBL_DIG; // digits of phys_lfloat

    constexpr phys_float PHYS_FLOAT_EPSILON = DBL_EPSILON;

    /**
     * @brief Presision with angles. If a given angle is less or equal to this value
     * then it is 0.
     * 
     */
    constexpr phys_float PHYS_FLOAT_ANGLE_EPSILON = 2.10734242554471e-08;

    /**
     * @brief phys_float maximum value 
     * 
     */
    constexpr phys_float PHYS_FLOAT_MAX = DBL_MAX;

    /**
     * @brief phys_float minimum value 
     * 
     */
    constexpr phys_float PHYS_FLOAT_MIN = DBL_MIN;

    /**
     * @brief Definition of +infinity
     * 
     */
    constexpr phys_float PLUS_INF = PHYS_FLOAT_MAX;

    /**
     * @brief Definition of -infinity
     * 
     */
    constexpr phys_float MINUS_INF = PHYS_FLOAT_MIN;

    /*****************************************************************************
    *                   USEFUL COMPILATION TIME FUNCTIONS
    ******************************************************************************/

    /**
     * @brief left <= value <= right
     * 
     * Check if the given value is on the given closed interval.
     *                  
     * 
     * @tparam T Type of the data objects: left, right.
     * @param left      Greatest lower bound.
     * @param value     Value.
     * @param right     Lowest upper bound.
     * @return constexpr phys_bool - true if the value is on the given interval,
     *                   otherwise false.
     * 
     * @see checkRangeExclusive() checkRangeInclusiveLeft() checkRangeInclusiveRight()
     */
    template <typename T>
    constexpr phys_bool checkRangeInclusive(const T &left, const T &value, const T &right)
    {
        return (left <= value) && (value <= right);
    }

    /**
     * @brief left < value < right
     * 
     * Check if the given value is on the given open interval.
     *                  
     * 
     * @tparam T Type of the data objects: left, right.
     * @param left      Greatest lower bound.
     * @param value     Value.
     * @param right     Lowest upper bound.
     * @return constexpr phys_bool - true if the value is on the given interval,
     *                   otherwise false.
     * 
     * @see checkRangeInclusive() checkRangeInclusiveLeft() checkRangeInclusiveRight()
     */
    template <typename T>
    constexpr phys_bool checkRangeExclusive(const T &left, const T &value, const T &right)
    {
        return (left < value) && (value < right);
    }

    /**
     * @brief left <= value < right.
     * 
     * Check if the given value is on the given interval, which is closed from the left
     * and open from the right.
     *                  
     * 
     * @tparam T Type of the data objects: left, right.
     * @param left      Greatest lower bound.
     * @param value     Value.
     * @param right     Lowest upper bound.
     * @return constexpr phys_bool - true if the value is on the given interval,
     *                   otherwise false.
     * 
     * @see checkRangeInclusive() checkRangeExclusive() 
     */
    template <typename T>
    constexpr phys_bool checkRangeInclusiveLeft(const T &left, const T &value, const T &right)
    {
        return (left <= value) && (value < right);
    }

    /**
     * @brief left < value <= right.
     * 
     * Check if the given value is on the given interval, which is opened from the left
     * and closed from the right.
     *                  
     * 
     * @tparam T Type of the data objects: left, right.
     * @param left      Greatest lower bound.
     * @param value     Value.
     * @param right     Lowest upper bound.
     * @return constexpr phys_bool - true if the value is on the given interval,
     *                   otherwise false.
     * 
     * @see checkRangeInclusive() checkRangeExclusive()
     */
    template <typename T>
    constexpr phys_bool checkRangeInclusiveRight(const T &left, const T &value, const T &right)
    {
        return (left < value) && (value <= right);
    }

    /**
     * @brief Calculate the square of an obj(number, scalar, vector, etc)
     * 
     * @tparam T Type of the obj.
     * @param value Object
     * @return decltype(value*value) 
     */
    template <typename T>
    constexpr auto sqr(const T &value) -> decltype(value * value) { return value * value; }

    /**
     * @brief Calculate the cube of an obj(scalar, vector, etc)
     * 
     * @tparam T Type of the obj
     * @param value Object
     * @return decltype(value * sqr(value)) 
     */
    template <typename T>
    constexpr auto cub(const T &value) -> decltype(value * sqr(value)) { return value * sqr(value); }

    /**
     * @brief Calculate the square root of a scalar
     * 
     * @tparam T type of the scalar
     * @param value - scalar
     * @return decltype(std::pow(value, 0.5)) 
     */
    template <typename T>
    constexpr auto sqrt(T value) -> decltype(std::pow(value, 0.5)) { return std::pow(value, 0.5); }

    /**
     * @brief Meter definition
     * 
     */
    constexpr phys_float operator"" _m(phys_lfloat x)
    {
        return x;
    }

    /**
     * @brief Second definition
     * 
     */
    constexpr phys_float operator"" _s(phys_lfloat x)
    {
        return x;
    }

    /**
     * @brief Kilogram definition
     * 
     */
    constexpr phys_float operator"" _kg(phys_lfloat x)
    {
        return x;
    }

    /**
     * @brief Convert cm to m.
     * 
     */
    constexpr phys_float operator"" _cm(phys_lfloat x)
    {
        return x / 100.0;
    }

    /**
     * @brief Convert cm2 to m2.
     * 
     */
    constexpr phys_float operator"" _cm2(phys_lfloat x)
    {
        return x / 10'000.0;
    }

    /**
     * @brief Convert eV to J.
     * 
     */
    constexpr phys_float operator"" _eV(phys_lfloat x)
    {
        return x * 1.602189E-19;
    }

    /**
     * @brief Convert eV to its equivalent value in Kelvin.
     * 
     */
    constexpr phys_float operator"" _eVToKelvin(phys_lfloat x)
    {
        return x * 11'605.0;
    }

    /**
     * @brief Literal operator for "Kilo = 1'000" 
     * 
     */
    constexpr phys_float operator"" _k(phys_lfloat x)
    {
        return x * 1000.0;
    }

    /**
     * @brief Literal operator for "Mega = 1'000'000" 
     * 
     */
    constexpr phys_float operator"" _M(phys_lfloat x)
    {
        return x * 1'000'000.0;
    }

    /**
     * @brief Calculate the factorial of a number if 
     *        it is possible at compile time. 
     * 
     * @param n number to get its factorial
     * @return constexpr phys_int n!
     */
    constexpr phys_int factorialCompile(phys_int n)
    {
        if (n < 0)
            return -1;

        if (n == 0 || n == 1)
            return 1;

        phys_int result{n};

        for (size_t i = n; i > 1; --i)
        {
            result *= (i - 1);
        }
        return result;
    }

    /**
     * @brief Calculate the integral of a a function *f* on \f$ [a,b] \f$.
     * 
     * @tparam T function type(it can be a single argument function or functor)
     * @tparam T_ type of the argument of the function(usually a floating point value)
     * @param function function to integrate
     * @param lowerBound integral lower bound
     * @param upperBound integral upper bound
     * @param numberOfIntervals number of intervals of integration(precision of the integration)
     * @return T_ the integral of the function
     */
    template <typename T, typename T_ = phys_float>
    constexpr T_ integrate(T function, T_ lowerBound, T_ upperBound, phys_size numberOfIntervals)
    {
        if (upperBound <= lowerBound)
        {
            return -1;
        }

        phys_float deltaX{(upperBound - lowerBound) / static_cast<phys_float>(numberOfIntervals)};

        phys_float result{};

        for (phys_size i = 0; i < numberOfIntervals; ++i)
        {
            result += (function(lowerBound + i * deltaX));
        }
        result *= deltaX;

        return result;
    }

    /**
     * @brief Degrees of an angle of 2π
     * 
     */
    constexpr phys_uint DEGREES_360 = 360;

    /**
     * @brief Number π
     * 
     */
    constexpr phys_float Pi = 3.14159265358979323846264;

    /**
     * @brief Gravitational-constant of Newton's law
     * 
     */
    constexpr phys_float G = 6.67E-11;

    /**
     * @brief Mass of the Earth
     * 
     */
    constexpr phys_float M_earth = 5.972E24;

    /**
     * @brief Mass of the Moon
     * 
     */
    constexpr phys_float M_moon = 7.349E22;

    /**
     * @brief Mass of the SUn
     * 
     */
    constexpr phys_float M_sun = 1.989E30;

    /**
     * @brief Mass of the electron
     * 
     */
    constexpr phys_float m_e = 0.911E-30;

    /**
     * @brief Distance between the Sun and the Earth
     * 
     */
    constexpr phys_float d_sun_earth = 149'597'870'700.0;

    /**
     * @brief Distance between the Earth and the Moon
     * 
     */
    constexpr phys_float d_earth_moon = 384'440'000.0;

    /**
     * @brief Planck constant
     * 
     */
    constexpr phys_float h_Plank = 6.626E-34;

    /**
     * @brief Rest energy of an electron
     * 
     */
    constexpr phys_float mc2_e = 0.511E6_eV;

    /**
     * @brief Boltzmann constant
     * 
     */
    constexpr phys_float k_B = 1.381E-23;

    /**
     * @brief Speed of light in vacuum
     * 
     */
    constexpr phys_float c_light = 299'792'458.0;

    /**
     * @brief Electron mass times the speed of light
     * 
     */
    constexpr phys_float mc_e = mc2_e / c_light;

    /**
     * @brief Thompson's scattering section
     * 
     */
    constexpr phys_float sigma_T = 6.652E-25_cm2;

    /***************************************************************
    *               MISCELLANEOUS
    ****************************************************************/

    /**
     * @brief Standard coordinate systems.
     * 
     */
    enum class eCoordSys
    {
        REC, // cartesian
        POL, //polar
        SPH  // spherical
    };

    /**
     * @brief This enum helps to determine intersection of lines and shapes.
     * 
     */
    enum class eSense
    {
        FORWARD,  // THE SAME SENSE AS THE VECTOR DIRECTION
        BACKWARD, // OPPOSITE TO THE VECTOR DIRECTION
        BOTH      // BOTH THE SAME AND THE OPPOSITE
    };

    /**
     * @brief This class represents the number of threads
     * 
     */
    enum class eNumOfThreads
    {
        ONE = 1U,
        TWO,
        THREE,
        FOUR,
        MAX
    };

    /**
     * @brief This is a tool for loading a file of n rows by m columns
     *        to a custom physapi container.
     * 
     * The elements of the container have to be able to be built from
     * rows of the file. The container need to have a method called 
     *              " loadElement(std::array) "
     * which receives an array of size equal columns and type T.
     * 
     * The number of raws depends on the number of raws of the file.
     * 
     * @tparam columns number of columns
     * @tparam Target target container's type
     * @tparam T one of the fundamental data types of in physapi(e.g. phys_float)
     * @param file file name
     * @param target container where the data will be stored
     * @param maxNunOfRaws maximum number of rows. If 0 then 
     *                     it depends on the file's number of rows.
     * 
     * @exception if any problem occurs while loading the file the this function
     *            abort the execution of the program.
     */
    template <phys_size columns, typename Target, typename T = phys_float>
    void loadFromFile(const std::string &file, Target &target, phys_size maxNunOfRows = 0)
    {
        std::ifstream fin;
        fin.open(file);
        if (!fin.good())
        {
            std::cout << "file " << file
                      << " not found!" << std::endl;
            abort();
        }

        std::array<T, columns> a;

        phys_size numberOfRows{};

        if (maxNunOfRows != 0)
        {
            while (!fin.eof() && numberOfRows < maxNunOfRows)
            {
                for (phys_size i = 0; i < columns; ++i)
                {
                    fin >> a[i];
                }

                if (!fin.good())
                {
                    break;
                }

                target.loadElement(a);

                numberOfRows++;
            }
        }
        else
        {
            while (!fin.eof())
            {
                for (phys_size i = 0; i < columns; ++i)
                {
                    fin >> a[i];
                }

                if (!fin.good())
                {
                    break;
                }
                target.loadElement(a);
            }
        }

        fin.close();
    }

    /**
     * @brief Degrees to radian
     * 
     */
    constexpr phys_float operator"" _deg(phys_lfloat x)
    {
        return x * Pi / 180.0;
    }

    /**
     * @brief Generate random numbers offering a simple interface.
     * 
     * This class handles the reneration of random numbers
     * and it offers a simple interface. You can create a global
     * instance of this class and pass references of it where you need
     * random number generation.
     * 
     */
    class PRandom
    {
    public:
        PRandom()
            : m_distribution{-1.0, 1.0}
        {
        }

        /**
         * @brief Get a random number between 0 and 1
         * 
         * This method is a wrapper of 
         *      std::generate_canonical<type,bits> 
         * 
         * @tparam bits precision
         * @return phys_float random number on (0,1)
         */
        template <phys_size bits = 14>
        phys_float random_0_1()
        {
            return std::generate_canonical<phys_float, bits>(m_rd);
        }

        /**
         * @brief Get a random number between -1 and +1
         * 
         * @return phys_float random number on (-1,1)
         */
        phys_float random_minusPlus_1() { return m_distribution(m_rd); }

        /**
         * @brief Get a random number between left and right bounds
         * 
         * @param left lower bound
         * @param right upper bound
         * @return phys_float random number on (left, right)
         */
        phys_float uniform_real(phys_float left, phys_float right)
        {
            std::uniform_real_distribution<phys_float> distribution(left, right);
            return distribution(m_rd);
        }

    private:
        std::random_device m_rd;
        std::uniform_real_distribution<phys_float> m_distribution;
    };

    /**
     * @brief This class process a generic table. It can load elements to it
     *        with defined constrains.
     * 
     * @tparam T_key type of the table elements keys
     * @tparam T_value type of the table elements values
     * @tparam size number of table columns
     * @tparam T_LoadArray type of the table loader functor
     */
    template <typename T_key, typename T_value, const phys_size SIZE_phys, typename T_LoadArray>
    class PProcessTable
    {
        class t_Table_ : public std::vector<std::pair<T_key, T_value>>
        {
        public:
            void loadElement(const std::array<phys_float, SIZE_phys> &a)
            {
                this->push_back(std::move(loader(a)));
            }

        private:
            T_LoadArray loader;
        };

    public:
        /**
         * @brief Construct a new PProcessTable object
         * 
         * @param file file name
         */
        PProcessTable(const std::string &file) { loadFromFile<SIZE_phys>(file, m_Table); }

        /**
         * @brief Construct a new PProcessTable object
         * 
         * @tparam T_Config special filter type
         * @param file file name
         * @param f filter
         */
        template <typename T_Config>
        PProcessTable(const std::string &file, const T_Config &f)
        {
            loadFromFile<SIZE_phys>(file, m_Table);

            f(m_Table);
        }

        /**
         * @brief Load the table with the data
         * 
         * @tparam T_Table Table type
         * @tparam T_predicate type of the predicate
         * @tparam Args variadic argument types
         * @param table table which will be loaded with the data
         * @param predicate predicate filter
         * @param args arguments
         */
        template <typename T_Table, typename T_predicate, typename... Args>
        void operator()(T_Table &table, T_predicate predicate, const Args &... args)
        {
            for (size_t i = 0; i < m_Table.size(); ++i)
            {
                //only if the functor-predicate returns true
                //then we set that "element" to the table
                if (predicate(m_Table[i], args...))
                {
                    table[m_Table[i].first] = m_Table[i].second;
                }
            }
            m_Table.clear();
        }

    private:
        t_Table_ m_Table;
    };

    /**
     * @brief Get the solid angle of a cone with a given 
     * half opening angle theta.
     * 
     * @param theta half opening angle
     * @return phys_float solid angle
     */
    inline phys_float solidAngle(phys_float theta)
    {
        return 2 * Pi * (1.0 - std::cos(theta));
    }

    /**@}*/

} // namespace physapi
