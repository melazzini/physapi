#include "PSphere.hpp"

namespace physapi
{

    phys_bool PSphere::isInside(const PPosition &point) const
    {
        return (position().distanceToPoint(point) <= m_radius);
    }

    phys_bool PSphere::intersect(const PPosition &position_,
                                 const PVector3D &direction,
                                 eSense sense) const
    {
        if (eSense::FORWARD == sense)
        {
            return intersectForward(position_, direction);
        }
        else if (eSense::BACKWARD == sense)
        {
            return intersectForward(position_, -direction);
        }
        else if (eSense::BOTH == sense)
        {
            return (intersectForward(position_, direction) || intersectForward(position_, -direction));
        }

        return false;
    }

    phys_bool PSphere::intersect(const PSphere &other) const
    {
        auto distanceBetweenSpheres = (other.position() - position()).norm();
        return (distanceBetweenSpheres < (m_radius + other.m_radius));
    }

    phys_float PSphere::distanceToBoundaryFromOutside(const PPosition &position_,
                                                      const PVector3D &direction) const
    {
        if (isInside(position_))
        {
            return -1;
        }

        // the order is important!
        auto dr = this->position() - position_;

        if(dr.norm()==0)
        {
            return 0;
        }

        auto alpha = PVector3D::angleBetweenVectors(dr / dr.norm(), direction);

        if(alpha>Pi/2)
        {
            return -1;
        }

        if (cos(alpha) <= 0)
        {
            dr = -dr;
            alpha = PVector3D::angleBetweenVectors(dr, direction);
        }

        // height or h is the opposite leg of the triangle
        auto h = dr.norm() * std::sin(alpha);

        if (h > m_radius)
        {
            return -1;
        }

        // R*cos(beta) = height"="h
        auto beta = std::acos(h / m_radius);

        // where: alpha + beta + gamma = Pi/2
        auto gamma = (Pi / 2) - alpha - beta;

        // where: dr - R*cos(alpha) = d*cos(alpha)

        // we return d
        return (dr.norm() - m_radius * std::cos(gamma)) / cos(alpha);
    }

    phys_float PSphere::distanceToBoundaryFromInside(const PPosition &position_,
                                                     const PVector3D &direction) const
    {
        if (!isInside(position_))
        {
            return -1;
        }

        // the order is important!
        auto dr = this->position() - position_;

        auto alpha = PVector3D::angleBetweenVectors(dr, direction);

        // height or h is the opposite leg of the triangle
        auto h = dr.norm() * std::sin(alpha);

        return dr.norm() * cos(alpha) + sqrt(sqr(m_radius) - sqr(h));
    }

    phys_float PSphere::distanceBetweenBoundaries(const PPosition &position_,
                                                  const PVector3D &direction) const
    {

        // the order is important!
        auto dr = this->position() - position_;

        auto alpha = PVector3D::angleBetweenVectors(dr, direction);

        // height or h is the opposite leg of the triangle
        auto h = dr.norm() * std::sin(alpha);

        if (h > m_radius)
        {
            return -1;
        }

        return 2.0 * sqrt(sqr(m_radius) - sqr(h));
    }

    phys_bool PSphere::intersectForward(const PPosition &position_,
                                        const PVector3D &direction) const
    {
        if (isInside(position_))
        {
            return true;
        }

        if (direction.norm() == 0)
        {
            return false;
        }

        auto adjacentLeg = position() - position_;

        auto alpha = PVector3D::angleBetweenVectors(direction, adjacentLeg);

        if (alpha > Pi / 2)
        {
            return false;
        }

        auto oppositeLegLength = std::abs(adjacentLeg.norm() * std::sin(alpha));

        return (oppositeLegLength < m_radius);
    }

} // namespace physapi
