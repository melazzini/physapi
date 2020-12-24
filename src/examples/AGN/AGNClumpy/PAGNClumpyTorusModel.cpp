#include"PAGNClumpyTorusModel.hpp"

namespace agn
{
	PAGNClumpyTorusModel::PAGNClumpyTorusModel(const PSimpleTorus& torus, phys_float N_H)
		:m_torus{torus}, m_N_H{N_H}
	{
	}
	phys_float PAGNClumpyTorusModel::equatorialEffectiveHalfSize() const
	{
		return phys_float();
	}
	phys_bool PAGNClumpyTorusModel::isInside(const PPosition& position) const
	{
		return phys_bool();
	}
	phys_float PAGNClumpyTorusModel::n_H() const
	{
		return phys_float();
	}
	const std::vector<PPosition>& PAGNClumpyTorusModel::clouds() const
	{
		return m_clouds;
	}
}// namespace agn