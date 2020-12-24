#pragma once

#include"PSpectralFormulaBase.hpp"

namespace agn
{
	using namespace physapi;


	class PAGNFormula : public PSpectralFormulaBase
	{
	private:

		phys_float m_E_low;

		phys_float m_E_cut;

		phys_float m_gamma;
	public:

		PAGNFormula(phys_float E_low, phys_float E_cut, phys_float gamma = 1.8)
			:m_E_low{E_low}, m_E_cut{E_cut}, m_gamma{gamma}
		{
		}

		// Inherited via PSpectralFormulaBase
		virtual phys_float operator()(phys_float energy) const override
		{
			return std::pow(energy, -m_gamma) * std::exp(-energy / m_E_cut);
		}

		phys_float E_low()const { return m_E_low; }
		phys_float E_cut()const { return m_E_cut; }
		phys_float gamma()const { return m_gamma; }

	};
}// namespace agn