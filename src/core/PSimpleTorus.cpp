#include "PSimpleTorus.hpp"

namespace physapi
{
    phys_bool PSimpleTorus::isInside(const PPosition &point) const
    {
        // first we wan to determine if the point is inside
        // the "internal sphere" or outside the external sphere
        auto dr = point - this->position();

        if (dr.norm() < m_innerRadius || dr.norm() > m_outerRadius)
        {
            return false;
        }

        // if we get to this code line it means that the point
        // is between the two concentric "spheres"
        // therefore we check the angular position
        if (std::abs(dr.phi()) > (Pi / 2 - m_theta))
        {
            return false;
        }

        return true;
    }

    phys_bool PSimpleTorus::intersect(const PPosition &position,
                                      const PVector3D &direction,
                                      eSense sense) const
    {
        /*
        todo:

        */

        return false;
    }

    phys_float PSimpleTorus::distanceToBoundaryFromOutside(const PPosition &position,
                                                           const PVector3D &direction) const
    {
        const double precision = maximumLength() / 100;

        for (phys_float ds_i = 0; ds_i < maximumLength(); ds_i += precision)
        {
            PVector3D r_0 = position + (ds_i * direction);

            if (isInside(PPosition(r_0)))
            {
                return ds_i;
            }
        }

        return -1;
    }

    phys_float PSimpleTorus::distanceToBoundaryFromInside(const PPosition &position,
                                                          const PVector3D &direction) const
    {
        const double precision = maximumLength() / 100;

        for (phys_float ds_i = 0; ds_i < maximumLength(); ds_i += precision)
        {
            PVector3D r_0 = position + (ds_i * direction);

            if (!isInside(PPosition(r_0)))
            {
                return ds_i;
            }
        }

        return -1;
    }

    phys_float PSimpleTorus::distanceBetweenBoundaries(const PPosition &position,
                                                       const PVector3D &direction) const
    {
        /*
        todo:

        */

        return -1;
    }

    phys_float PSimpleTorus::volume() const
    {
        // volume of the outer sphere
        phys_float V_R2 = (4. / 3.) * Pi * cub(m_outerRadius);

        // volume of the inner sphere
        phys_float V_R1 = (4. / 3.) * Pi * cub(m_innerRadius);

        // volume of the spherical shape corresponging
        // to the half opening angle
        phys_float V_theta = (4. / 3.) * Pi * (1 - std::cos(m_theta)) * (cub(m_outerRadius) - cub(m_innerRadius));

        return V_R2 - (V_theta + V_R1);
    }

} // namespace physapi
