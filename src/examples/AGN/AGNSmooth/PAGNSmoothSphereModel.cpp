#include"PAGNSmoothSphereModel.hpp"

namespace agn
{
	PAGNSmoothSphereModel::PAGNSmoothSphereModel(const PSphere& sphere, phys_float N_H)
		:m_sphere{ m_sphere }, m_N_H{ N_H }
	{
	}
	std::optional<phys_float> PAGNSmoothSphereModel::distanceToBoundary(const PPosition& position, const PVector3D& direction) const
	{
		auto d = m_sphere.distanceToBoundaryFromInside(position, direction);
		if (d >= 0.0)
		{
			return d;
		}
		else
			return std::nullopt;
	}
	std::optional<phys_float> PAGNSmoothSphereModel::distanceToEnterinPoint(const PPosition& position, const PVector3D& direction) const
	{
		auto d = m_sphere.distanceToBoundaryFromOutside(position, direction);
		if (d >= 0.0)
		{
			return d;
		}
		else
			return std::nullopt;
	}
	phys_float PAGNSmoothSphereModel::equatorialEffectiveHalfSize() const
	{
		return m_sphere.radius();
	}
	phys_bool PAGNSmoothSphereModel::isInside(const PPosition& position) const
	{
		return m_sphere.isInside(position);
	}
	phys_float PAGNSmoothSphereModel::n_H() const
	{
		return m_N_H / m_sphere.radius();
	}
}