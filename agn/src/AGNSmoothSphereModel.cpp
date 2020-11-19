#include "AGNSmoothSphereModel.hpp"

namespace agn
{
    phys_float AGNSmoothSphereModel::distanceToBoundary(const PPosition &position, const PVector3D &direction) const
    {
        return m_sphere.distanceToBoundaryFromInside(position, direction);
    }

    phys_float AGNSmoothSphereModel::distanceToEnteringPoint(const PPosition &position,
                                                             const PVector3D &direction) const
    {
        return m_sphere.distanceToBoundaryFromOutside(position, direction);
    }
} // namespace agn
