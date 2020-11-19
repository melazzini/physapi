#pragma once

#include "PPosition.hpp"

namespace physapi
{
    /**
     * @brief This is the base class for all 3D geometric
     * forms(cubs, spheres, tori, etc.)
     * 
     * @ingroup core
     * 
     */
    class PGeometryBase
    {
    private:
        // position of the geometry
        PPosition m_position;

    public:
        /**
         * @brief Construct a new PGeometryBase object
         * 
         * @param position 
         */
        explicit PGeometryBase(const PPosition &position = PPosition())
            : m_position(position)
        {
        }

        /**
         * @brief Destroy the PGeometryBase object
         * 
         */
        virtual ~PGeometryBase() {}

        /**
         * @brief Check if the given point is inside the geometry
         * 
         * @param point point
         * @return phys_bool true if the point is inside, else false
         */
        virtual phys_bool isInside(const PPosition &point) const = 0;

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
                                    eSense sense = eSense::FORWARD) const = 0;

        /**
         * @brief Get the distance to the geometry's boundaries from an outside point
         * 
         * @param position position of the point
         * @param direction direction
         * @return phys_float if the geometry is not in the path, given
         * by the position and direction then this method returns -1
         */
        virtual phys_float distanceToBoundaryFromOutside(const PPosition &position,
                                                         const PVector3D &direction) const = 0;

        /**
         * @brief Get the distance to the geometry's boundaries from an inside point
         * 
         * @param position position of the point
         * @param direction direction
         * @return phys_float 
         */
        virtual phys_float distanceToBoundaryFromInside(const PPosition &position,
                                                        const PVector3D &direction) const = 0;

        /**
         * @brief Get the distance from one boundary to another on the given line
         * 
         * @param position 
         * @param direction 
         * @return phys_float 
         */
        virtual phys_float distanceBetweenBoundaries(const PPosition &position,
                                                     const PVector3D &direction) const = 0;

        /**
         * @brief This class gives an estimation of the average size
         * 
         * @return phys_float estimation size
         */
        virtual phys_float estimationAverageSize() const = 0;

        /**
         * @brief Get the length inside the geometry starting from a position
         * 
         * @param position 
         * @param direction 
         * @return phys_float 
         * @return 0 if the line does not enters the geometry
         */
        virtual phys_float effectiveLength(const PPosition &position,
                                           const PVector3D &direction) const = 0;

        /**
         * @brief Get the length between boundaries, not inside the geometry
         * 
         * @param position 
         * @param direction 
         * @return phys_float 
         */
        virtual phys_float freeLength(const PPosition &position,
                                      const PVector3D &direction) const = 0;

        /**
         * @brief Get the volume of the geometry
         * 
         * @return phys_float 
         */
        virtual phys_float volume() const = 0;

        /**
         * @brief Get the position of the object
         * 
         * @return const PPosition& 
         */
        const PPosition &position() const { return m_position; }

        /**
         * @brief Set a new position
         * 
         * @param position_ 
         */
        void setPosition(const PPosition &position_) { m_position = position_; }

        /**
         * @brief Translate the geometry
         * 
         * @param dr 
         */
        void translate(const PPosition &dr) { m_position += dr; }
    };

} // namespace physapi
