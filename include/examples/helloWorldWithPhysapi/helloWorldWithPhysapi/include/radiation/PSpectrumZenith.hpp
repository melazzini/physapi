#pragma once

#include "PSpectrum.hpp"

namespace physapi
{
    /**
 * @brief This class represents a photon distribution under a specfic
 * zenith angle and an angular interval obtained from a simulation file.
 * 
 * You can load the spectrum from a data file, resulting from the
 * corresponding simulation using physapi::loadFromFile().
 * 
 * @tparam intervals energy intervals
 * @tparam cols number of columns
 * 
 * @ingroup radiation
 */
    template <phys_size intervals, phys_size cols>
    class PSpectrumZenith : public PSpectrum<intervals>
    {
    private:
        phys_float m_minPhi;
        phys_float m_maxPhi;

    public:
        /**
     * @brief Construct a new PSpectrumZenith object
     * 
     * @param zenithAngle zenith angle
     * @param E_low Energy lower bound
     * @param E_upp Energy upper bound
     * @param scale scale
     * @param angularInterval angular interval
     */
        PSpectrumZenith(
            phys_float zenithAngle,
            phys_float E_low,
            phys_float E_upp,
            eSpectrumScale scale = eSpectrumScale::LIN,
            phys_float angularInterval = 10.0_deg)
            : PSpectrum<intervals>(E_low, E_upp, scale)
        {
            m_maxPhi = Pi / 2 - zenithAngle;
            m_minPhi = Pi / 2 - (zenithAngle + angularInterval);

            if (m_maxPhi < m_minPhi || m_minPhi < 0)
            {
                std::cerr << "PSpectrumZenith:   bad angles!" << std::endl;
                std::cerr << "Zenith: " << zenithAngle << std::endl;
                std::cerr << "AngularInterval: " << angularInterval << std::endl;
                std::cerr << "The sum of both must be less than or equal to pi/2." << std::endl;
                std::cerr << "In addition, the angular interval cannot be negative." << std::endl;
                std::abort();
            }
        }

        /**
     * @brief Get the absolute value of the minimum phi angle
     * 
     * @return phys_float 
     */
        phys_float minPhi() const { return m_minPhi; }

        /**
     * @brief Get the absolute value of the the maximum phi angle
     * 
     * @return phys_float 
     */
        phys_float maxPhi() const { return m_maxPhi; }

        /**
     * @brief This method loads an element(counts a photon) to the spectrum.
     * 
     * This method can be used with physapi::loadFromFile()
     * 
     * @param row a row from the corresponding file
     */
        virtual void loadElement(const std::array<phys_float, cols> &row) = 0;
    };
} // namespace physapi
