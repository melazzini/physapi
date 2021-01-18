#include"PAGNSmoothTorus.model.hpp"

namespace agn
{
	PAGNSmoothTorusModel::PAGNSmoothTorusModel(const PSimpleTorus& torus, phys_float N_H)
		:m_torus{torus}, m_N_H{N_H}
	{
	}

	std::optional<phys_float> PAGNSmoothTorusModel::distanceToBoundary(const PPosition& position, const PVector3D& direction) const
	{
		auto d = m_torus.distanceToBoundaryFromInside(position, direction);
		if (d >= 0.0)
		{
			return d;
		}
		else
			return std::nullopt;
	}

	std::optional<phys_float> PAGNSmoothTorusModel::distanceToEnteringPoint(const PPosition& position, const PVector3D& direction) const
	{
		auto d = m_torus.distanceToBoundaryFromOutside(position, direction);
		if (d >= 0.0)
		{
			return d;
		}
		else
			return std::nullopt;
	}

	phys_float PAGNSmoothTorusModel::equatorialEffectiveHalfSize() const
	{
		return m_torus.externalRadius()-m_torus.internalRadius();
	}

	phys_bool PAGNSmoothTorusModel::isInside(const PPosition& position) const
	{
		return m_torus.isInside(position);
	}

	phys_float PAGNSmoothTorusModel::n_H() const
	{
		return m_N_H/equatorialEffectiveHalfSize();
	}


}// namespace agn