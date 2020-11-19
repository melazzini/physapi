#pragma once

#include "AGNStructureModel.hpp"

namespace agn
{
    /**
     * @brief This represents a clumpy toroidal structure for AGN
     * 
     * @ingroup agn
     * 
     */
    class AGNClumpyTorusModel : public AGNStructureModel
    {
    private:
        // torus
        PSimpleTorus m_torus;

        // column density
        phys_float m_N_H;

        // average number of clouds along the equatorial line of sight
        phys_float m_N_aver;

        // volume filling factor
        phys_float m_phi;

        // clouds
        std::vector<PVector3D> m_clouds;

    public:
        /**
         * @brief Construct a new AGNClumpyTorusModel object
         * 
         * @param torus geometric box, inside of which the clouds will be placed
         * @param N_H equatorial average column density
         * @param N_aver average number of clouds along the equatorial line of sight
         * @param phi volume filling factor
         */
        AGNClumpyTorusModel(const PSimpleTorus &torus, phys_float N_H, phys_float N_aver, phys_float phi);

        /**
         * @brief Destroy the AGNClumpyTorusModel object
         * 
         */
        ~AGNClumpyTorusModel() {}

        /**
         * @brief Get the distance to the next boundary inside the agn.
         * 
         * @param position reference point
         * @param direction direction of the trajectory
         * @return phys_float distance to the next boundary
         * @return -1   if there is no boundary for the given direction and reference point. 
         */
        virtual phys_float distanceToBoundary(const PPosition &position, const PVector3D &direction) const override;

        /**
         * @brief Get the distance to the next entering point of the agn structure
         * 
         * @param position reference point
         * @param direction direction of the trajectory
         * @return phys_float distance to the entering point
         * @return -1 there is not entering point for the given arguments
         */
        virtual phys_float distanceToEnteringPoint(const PPosition &position,
                                                   const PVector3D &direction) const override;

        /**
         * @brief Get the equatorial half size of the agn structure model
         * 
         * @return phys_float 
         */
        virtual phys_float equatorialEffectiveHalfSize() const
        {
            return -1;
        }

        /**
         * @brief Returns true if the given point is inside the structure, 
         * otherwise false
         * 
         * @param position point position
         * @return phys_bool 
         */
        virtual phys_bool isInside(const PPosition &position) const override;

        /**
         * @brief Get the concentration of hydrogen in the clouds
         * 
         * @return phys_float 
         */
        virtual phys_float n_H() const
        {
            return m_n_H;
        }

    private:
        PRandom m_rndMng;

        // average column density of the clouds
        phys_float m_N_H_clouds;

        // concentration of hydrogen
        phys_float m_n_H;

        // radius of the clouds
        phys_float m_cloudsRadius;

        // tot number of clouds
        phys_float m_N_clouds_tot;

        // index of the current(or last) cloud,
        // where the photon is located
        phys_int m_currentCloudIndex;

    private:
        phys_float calculateN_H_clouds() const
        {
            return m_N_H / m_N_aver;
        }

        phys_float calculate_n_H() const
        {
            return m_N_H / (m_phi * (m_torus.externalRadius() - m_torus.internalRadius()));
        }

        phys_float calculateCloudsRadius() const
        {
            return (3. / 4.) * (m_N_H / (m_n_H * m_N_aver));
        }

        phys_float calculateNumOfClouds() const
        {
            return (3. / 4.) * (m_phi / Pi) * (m_torus.volume() / cub(m_cloudsRadius));
        }

        void placeCloud(PVector3D &cloudPos);

        phys_bool noIntersection(const std::vector<phys_float> &d) const;

        void createClouds();

        phys_int findCloud(const PPosition &pos, const PVector3D &direction) const;

        phys_int cloudIndex(const PPosition &pos) const;
    };
} // namespace agn
