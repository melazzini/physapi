#pragma once

#include "PGeometryBase.hpp"

namespace physapi
{
    /**
     * @brief This class represents a 3d sphere.
     * 
     * @ingroup core
     * 
     */
    class PSphere : public PGeometryBase
    {
    private:
        phys_float m_radius;

    public:
        /**
         * @brief Construct a new sphere
         * 
         * @param position 
         * @param radius_ 
         */
        PSphere(const PPosition &position = PPosition(), phys_float radius_ = 0.0)
            : PGeometryBase(position), m_radius{radius_}
        {
        }

        /**
         * @brief Destroy the PSphere object
         * 
         */
        virtual ~PSphere() {}

        /**
         * @brief Check if the given point is inside the geometry
         * 
         * @param point point
         * @return phys_bool true if the point is inside, else false
         */
        virtual phys_bool isInside(const PPosition &point) const override;

        /**
         * @brief Check if the line determined by a point and a direction
         * intersects or not the geometry
         * 
         * @param position point's position
         * @param direction direction of the line
         * @param sense sense of the line
         * @return phys_bool
         * 
         */
        virtual phys_bool intersect(const PPosition &position,
                                    const PVector3D &direction,
                                    eSense sense = eSense::FORWARD) const override;

        /**
         * @brief Check if the given geometry intersects with other geometry
         * 
         * @param other other geometry
         * @return phys_bool true if the geometries intersect, else false
         */
        virtual phys_bool intersect(const PSphere &other) const;

        /**
         * @brief Get the distance to the geometry's boundaries from an outside point
         * 
         * @param position position of the point
         * @param direction direction
         * @return phys_float if the geometry is not in the path, given
         * by the position and direction then this method returns -1
         */
        virtual phys_float distanceToBoundaryFromOutside(const PPosition &position,
                                                         const PVector3D &direction) const override;

        /**
         * @brief Get the distance to the geometry's boundaries from an inside point
         * 
         * @param position position of the point
         * @param direction direction
         * @return phys_float 
         */
        virtual phys_float distanceToBoundaryFromInside(const PPosition &position,
                                                        const PVector3D &direction) const override;

        /**
         * @brief Get the distance from one boundary to another on the given line
         * 
         * @param position 
         * @param direction 
         * @return phys_float if the geometry is not in the path, given
         * by the position and direction then this method returns -1
         */
        virtual phys_float distanceBetweenBoundaries(const PPosition &position,
                                                     const PVector3D &direction) const override;

        /**
         * @brief This class gives an estimation of the average size
         * 
         * @return phys_float estimation size
         */
        virtual phys_float estimationAverageSize() const
        {
            return 2 * m_radius;
        }

        /**
         * @brief Get the length inside the geometry starting from a position
         * 
         * @param position 
         * @param direction 
         * @return phys_float 
         * @return 0 if the line does not enters the geometry
         */
        virtual phys_float effectiveLength(const PPosition &position,
                                           const PVector3D &direction) const override
        {
            if (isInside(position))
            {
                return distanceToBoundaryFromInside(position, direction);
            }
            else
                return distanceBetweenBoundaries(position, direction);
        }

        /**
         * @brief Get the length between boundaries, not inside the geometry
         * 
         * @param position 
         * @param direction 
         * @return phys_float 
         */
        virtual phys_float freeLength(const PPosition &position,
                                      const PVector3D &direction) const override
        {
            return 0;
        }

        /**
         * @brief Set the Radius object
         * 
         * @param r 
         */
        void setRadius(phys_float r) { m_radius = r; }

        phys_float radius() const { return m_radius; }

        virtual phys_float volume() const override
        {
            return (4. / 3.) * Pi * cub(m_radius);
        }

    private:
        phys_bool intersectForward(const PPosition &position,
                                   const PVector3D &direction) const;
    };
} // namespace physapi
