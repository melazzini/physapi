#pragma once

#include "PGeometryBase.hpp"
#include "PSphere.hpp"

namespace physapi
{
    /**
     * @brief This class represents a simple torus.
     * 
     * A simple torus in our api is defined by:
     *  
     *  * inner and outer radius
     *  * halfopening angle(\f$ \theta_{0} \f$)
     * 
     * @ingroup core
     * 
     */
    class PSimpleTorus : public PGeometryBase
    {
    private:
        phys_float m_innerRadius;

        phys_float m_outerRadius;

        phys_float m_theta;

    public:
        /**
         * @brief Construct a new PSimpleTorus object
         * 
         * @param r1 inner radius
         * @param r2 outer radius
         * @param theta0 halfopening angle(0,\f$ \pi/2 \f$)
         * @param pos position of the center
         */
        PSimpleTorus(phys_float r1 = 0, phys_float r2 = 0, phys_float theta0 = 0,
                     const PPosition &pos = PPosition())
            : PGeometryBase{pos},
              m_innerRadius{r1}, m_outerRadius{r2}, m_theta{theta0}
        {
        }

        /**
         * @brief Destroy the PSimpleTorus object
         * 
         */
        virtual ~PSimpleTorus() {}

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
            return 2 * m_outerRadius;
        }

        /**
         * @brief Get the length inside the geometry starting from a position
         * 
         * @param position 
         * @param direction 
         * @return phys_float
         */
        virtual phys_float effectiveLength(const PPosition &position,
                                           const PVector3D &direction) const
        {
            /*
                TODO:

            */

            return 0;
        }

        /**
         * @brief Get the maximum possible length inside the geometry
         * 
         * @return phys_float 
         */
        virtual phys_float maximumLength() const
        {
            return 2 * m_outerRadius;
        }

        /**
         * @brief Get the length between boundaries, not inside the geometry
         * 
         * @param position 
         * @param direction 
         * @return phys_float 
         */
        virtual phys_float freeLength(const PPosition &position,
                                      const PVector3D &direction) const
        {
            return 0;
        }

        phys_float externalRadius() const { return m_outerRadius; }

        phys_float internalRadius() const { return m_innerRadius; }

        phys_float theta0() const { return m_theta; }

        /**
         * @brief Get the volume of the geometry
         * 
         * @return phys_float 
         */
        virtual phys_float volume() const override;
    };
} // namespace physapi
