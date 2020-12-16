#pragma once

#include "radiation_utils.hpp"
#include <iostream>
#include <algorithm>
#include "PSimplePhoton.hpp"

namespace physapi
{
    /**
     * @brief This class represents a raw spectrum(or photon distribution).
     * 
     * @tparam energyIntervals number of energy intervals.
     * 
     * A raw spectrum is simply a vector of energy-value pairs
     * where the energy corresponds to the energy interval
     * and the value is the number of photons in that interval.
     * 
     * @ingroup radiation
     * 
     * **Example**
     * @code{.cpp}
        int main()
        {
            PSpectrum<10> spectrum{3, 20, eSpectrumScale::LIN};

            spectrum.addPhoton(4);
            spectrum.addPhoton(5);
            spectrum.addPhoton(9.9);
            spectrum.addPhoton(10);
            spectrum.addPhoton(7.8);

            cout << spectrum << endl;

            return 0;
        }
        @endcode

        output:

        0    3.85   1

        1    5.55   1
        
        2    7.25   1
        
        3    8.95   0
        
        4    10.65   2
        
        5    12.35   0
        
        6    14.05   0
        
        7    15.75   0
        
        8    17.45   0
        
        9    19.15   0

        @code{.cpp}

        #include <iostream>
        #include "PSpectrum.hpp"

        using namespace std;
        using namespace physapi;

        int main()
        {
            PSpectrum<10> spectrum{3, 20, eSpectrumScale::LOG};

            spectrum.addPhoton(4);
            spectrum.addPhoton(5);
            spectrum.addPhoton(9.9);
            spectrum.addPhoton(10);
            spectrum.addPhoton(7.8);

            cout << spectrum << endl;

            return 0;
        }

        @endcode

        output:

        0    3.31335   0

        1    4.00552   1
        
        2    4.84227   1
        
        3    5.85383   0
        
        4    7.0767   0
        
        5    8.55504   1
        
        6    10.3422   2
        
        7    12.5027   0
        
        8    15.1145   0
        
        9    18.272   0

     * 
     */
    template <phys_size energyIntervals>
    class PSpectrum
    {
    private:
        // spectrum data
        t_spectrum<energyIntervals> m_spectrum;

        // energy left bound
        phys_float m_leftBound;

        // energy right bound
        phys_float m_rightBound;

        // spectrum scale
        eSpectrumScale m_scale;

        // remaining photons on the left of the interval
        // not taken into the spectrum
        phys_float m_leftRemainder;

        // remaining photons on the right of the interval
        // not taken into the spectrum
        phys_float m_rightRemainder;

        // if the scale is linear then dE is the delta energy
        // and for each interval it is the same, but if the scale is
        // logarithmic then dE means the delta exponent or dExp
        // and the deltaEnergy will be 10^{dExp*index+1} - 10^{dExp*index}
        // ! do not change this value yourself use calculateDE() instead
        phys_float m_dE;

    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new empty spectrum
         * 
         */
        explicit PSpectrum(phys_float E_low, phys_float E_upp, eSpectrumScale scale = eSpectrumScale::LIN)
            : m_spectrum{},
              m_leftBound{E_low}, m_rightBound{E_upp}, m_scale{scale},
              m_dE{calculateDE(E_low, E_upp, scale)}
        {
            if (!energyBoundsGood(E_low, E_upp))
            {
                std::cerr << "PSpectrum constructor:  "
                          << "bad energy intervals!"
                          << std::endl;
                std::cerr << "E_low:  " << E_low << ",        E_upp: " << E_upp
                          << std::endl;
                std::abort();
            }

            makeEmptySpectrum(m_scale);
        }

        /**
         * @brief Destroy the spectrum
         * 
         */
        virtual ~PSpectrum() {}

        /***********************************************************************
        *              GETTERS & SETTERS
        ***********************************************************************/

        /**
         * @brief Clear the spectrum.
         * 
         * After this the spectrum will be intialized to zero in each interval.
         * 
         */
        void clear()
        {
            for (auto &&item : m_spectrum)
            {
                item.second = 0;
            }
        }

        /**
         * @brief Get the spectrum data
         * 
         * @return const t_spectrum& 
         */
        const t_spectrum<energyIntervals> &spectrum() const { return m_spectrum; }

        /**
         * @brief Get the left energy bound of the spectrum
         * 
         * @return phys_float 
         */
        phys_float leftBound() const { return m_leftBound; }

        /**
         * @brief Get the right energy bound of the spectrum
         * 
         * @return phys_float 
         */
        phys_float rightBound() const { return m_rightBound; }

        /**
         * @brief Get the spectrum scale
         * 
         * @return eSpectrumScale 
         */
        eSpectrumScale scale() const { return m_scale; }

        /**
         * @brief Get the remaining photons to the left of the interval.
         * 
         * This photons are not taken registered in the spectrum
         * 
         * @return phys_float 
         */
        phys_float leftRemainder() const { return m_leftRemainder; }

        /**
         * @brief Get the remaining photons to the right of the interval.
         * 
         * This photons are not taken registered in the spectrum
         * 
         * @return phys_float 
         */
        phys_float rightRemainder() const { return m_rightRemainder; }

        /**
         * @brief Get the remaining photons to the left and right of the interval.
         * 
         * This photons are not taken registered in the spectrum
         * 
         * @return phys_float 
         */
        phys_float remainder() const { return m_leftRemainder + m_rightRemainder; }

        /**
         * @brief Add a photon with energy hv to the spectrum
         * 
         * @param hv photon's energy
         * @return bool true if the photon has been added successfully,
         * false if not
         */
        bool addPhoton(phys_float hv)
        {
            if (hv <= 0)
            {
                return false;
            }

            if (hv < m_leftBound)
            {
                m_leftRemainder++;
                return false;
            }

            if (hv > m_rightBound)
            {
                m_rightRemainder++;
                return false;
            }

            phys_size index{};

            if (eSpectrumScale::LIN == m_scale)
            {
                index = static_cast<phys_size>((hv - m_leftBound) / m_dE);
            }
            else if (eSpectrumScale::LOG == m_scale)
            {
                index = static_cast<phys_size>((std::log10(hv) - std::log10(m_leftBound)) / m_dE);
            }
            else
            {
                return false;
            }

            m_spectrum[index].second++;

            return true;
        }

        /**
         * @brief Add a number of photons to the given interval
         * 
         * @param index interval index
         * @param n number of photons
         * @return true the photons were added to the spectrum successfully
         * @return false the photons were not added to the spectrum
         */
        bool addPhotons(phys_size index, phys_float n = 1)
        {
            if (index >= m_spectrum.size())
            {
                return false;
            }

            m_spectrum[index].second += n;

            return true;
        }

        /**
         * @brief Add a photon with its energy to the spectrum
         * 
         * @param photon photon
         * @return bool true if the photon has been added successfully,
         * false if not
         */
        bool addPhoton(const PSimplePhoton &photon)
        {
            return addPhoton(photon.energy());
        }

        template <size_t numberOfEnergyIntervals>
        friend std::ostream &operator<<(std::ostream &os, const PSpectrum<numberOfEnergyIntervals> &spectrum);

        /**
         * @brief Get the energy of the corresponding interval
         * 
         * @param index index of the interval
         * @return phys_float energy of the interval, -1 if any error
         */
        phys_float energy(phys_size index) const
        {
            if (index >= m_spectrum.size())
            {
                return -1;
            }

            return m_spectrum[index].first;
        }

        /**
         * @brief Get the energy lower bound of the interval determined by
         * the index.
         * 
         * @param index index of the energy interval
         * @return phys_float energy lower bound of the subinterval
         * @return -1 if any error
         */
        phys_float subintervalEnergyLowerBound(phys_size index) const
        {
            if (index >= m_spectrum.size())
            {
                return -1;
            }

            if (eSpectrumScale::LIN == m_scale)
            {
                return m_leftBound + m_dE * static_cast<phys_float>(index);
            }
            else if (eSpectrumScale::LOG == m_scale)
            {
                return m_leftBound * std::pow(10, m_dE * static_cast<phys_float>(index));
            }

            return -1;
        }

        /**
         * @brief Get the energy upper bound of the interval determined by
         * the index.
         * 
         * @param index index of the energy interval
         * @return phys_float energy upper bound of the subinterval
         * @return -1 if any error
         */
        phys_float subintervalEnergyUpperBound(phys_size index) const
        {
            if (index >= m_spectrum.size())
            {
                return -1;
            }

            if (eSpectrumScale::LIN == m_scale)
            {
                return m_leftBound + m_dE * static_cast<phys_float>(index + 1);
            }
            else if (eSpectrumScale::LOG == m_scale)
            {
                return m_leftBound * std::pow(10, m_dE * static_cast<phys_float>(index + 1));
            }

            return -1;
        }

        /**
         * @brief Get the spectrum value of the corresponding interval
         * 
         * @param index index of the interval
         * @return phys_float number of photons in the interval, -1 if any error
         */
        phys_float value(phys_size index) const
        {
            if (index >= m_spectrum.size())
            {
                return -1;
            }

            return m_spectrum[index].second;
        }

        /**
         * @brief Get the index that corresponds to the given energy
         * 
         * @param energy photon energy
         * @return phys_lint index
         * @return -1 if any error
         */
        phys_lint index(phys_float energy) const
        {
            if (energy <= 0)
            {
                return -1;
            }

            if (energy < m_leftBound)
            {
                return -1;
            }

            if (energy > m_rightBound)
            {
                return -1;
            }

            if (eSpectrumScale::LIN == m_scale)
            {
                return static_cast<phys_lint>((energy - m_leftBound) / m_dE);
            }
            else if (eSpectrumScale::LOG == m_scale)
            {
                return static_cast<phys_lint>((std::log10(energy) - std::log10(m_leftBound)) / m_dE);
            }
            else
            {
                return -1;
            }
        }

        /**
         * @brief Get the total number of photons in the spectrum
         * 
         * @return phys_size 
         */
        phys_size count() const
        {
            phys_size total{0};
            for (auto &&item : m_spectrum)
            {
                total += item.second;
            }
            return total;
        }

        PSpectrum operator+(const PSpectrum &other)
        {
            PSpectrum result{other.leftBound(), other.rightBound(), other.scale()};

            for (size_t i = 0; i < energyIntervals; i++)
            {
                result.addPhotons(i, m_spectrum[i].second + other.m_spectrum[i].second);
            }

            return result;
        }

    private:
        // returns true if the energy bounds are ok.
        // Otherwise false.
        bool energyBoundsGood(phys_float E_low, phys_float E_upp) const
        {
            return !(E_upp <= E_low || E_low < 0);
        }

        // calculate dE according to the scale type(lin or log)
        // it returns -1 if some error occurs
        phys_float calculateDE(phys_float E_low, phys_float E_upp, eSpectrumScale scale)
        {
            if (eSpectrumScale::LIN == scale)
            {
                return (E_upp - E_low) / static_cast<phys_float>(energyIntervals);
            }
            else if (eSpectrumScale::LOG == scale)
            {
                return (std::log10(E_upp) - std::log10(E_low)) / static_cast<phys_float>(energyIntervals);
            }
            else
            {
                return -1;
            }
        }

        // create an empty spectrum(zero photons in every interval)
        void makeEmptySpectrum(eSpectrumScale scale)
        {
            if (eSpectrumScale::LIN == scale)
            {
                makeLinEmptySpectrum();
            }
            else if (eSpectrumScale::LOG == scale)
            {
                makeLogEmptySpectrum();
            }
        }

        void makeLinEmptySpectrum()
        {
            /*

            when using a linear scale the energy of each interval is the mean:
                    E_i = (E_left_i + E_right_i)/2 = (E_left_i + E_left_i + dE)/2 
                        = (2E_left_i + dE)/2

                    E_i = E_left_i + (dE/2)

            where dE = (E_upp - E_left)/energyIntervals = constant for each interval

            */

            phys_float E_left_i = m_leftBound;

            for (size_t i = 0; i < m_spectrum.size(); i++)
            {
                m_spectrum[i].first = E_left_i + (m_dE / 2);

                E_left_i += m_dE;
            }
        }

        void makeLogEmptySpectrum()
        {
            /*

            when using a logarithmic scale the energy of each interval is the mean:
                    E_i = (E_left_i + E_right_i)/2 = (E_left_i + E_left_i + dE)/2 
                        = (2E_left_i + dE)/2

                    E_i = E_left_i + (dE/2)

            where dE = E_left_{i+1} - E_left_i

            E_left_{i+1} = E_left * 10^{dExp*(index + 1)}
            E_left_i = E_left * 10^{dExp*(index)}

            and 

            dExp = (log(E_right) - log(E_left))/energyIntervals

            */
            phys_float E_left_i = m_leftBound;

            for (size_t i = 0; i < m_spectrum.size(); i++)
            {
                phys_float E_left_ip1 = m_leftBound * std::pow(10, m_dE * (i + 1));

                phys_float E_left_i = m_leftBound * std::pow(10, m_dE * i);

                auto dE = E_left_ip1 - E_left_i;

                m_spectrum[i].first = E_left_i + (dE / 2);
            }
        }
    };

    template <size_t numberOfEnergyIntervals>
    std::ostream &operator<<(std::ostream &os, const PSpectrum<numberOfEnergyIntervals> &spectrum)
    {

        for (size_t i = 0; i < spectrum.m_spectrum.size(); i++)
        {
            os << spectrum.m_spectrum[i].first << "   "
               << spectrum.m_spectrum[i].second;

            if ((i + 1) < spectrum.m_spectrum.size())
            {
                os << std::endl;
            }
        }

        return os;
    }

} // namespace physapi
