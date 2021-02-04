#pragma once

#include"PSpectralFormulaBase.hpp"

namespace agn
{
	using namespace physapi;

	/**
	 * @brief This class represents an AGN spectral formula.
	 * 
	 * @ingroup agn
	 */
	class PAGNFormula : public PSpectralFormulaBase
	{
	private:

		phys_float m_E_low;

		phys_float m_E_cut;

		phys_float m_gamma;
	public:

		/**
		 * @brief Creates an agn formula object.
		 * 
		 * \param E_low energy lower bound
		 * \param E_cut energy upper bound
		 * \param gamma exponent
		 */
		PAGNFormula(phys_float E_low, phys_float E_cut, phys_float gamma = 1.8)
			:m_E_low{E_low}, m_E_cut{E_cut}, m_gamma{gamma}
		{
		}

		/**
		 * @brief This operator returns the photon distribution value for the given energy.
		 * 
		 * \param energy
		 * \return 
		 */
		virtual phys_float operator()(phys_float energy) const override
		{
			return std::pow(energy, -m_gamma) * std::exp(-energy / m_E_cut);
		}

		/**
		 * @brief Get the energy lower bound.
		 * 
		 * \return 
		 */
		phys_float E_low()const { return m_E_low; }

		/**
		 * @brief Get the energy upper bound.
		 * 
		 * \return 
		 */
		phys_float E_cut()const { return m_E_cut; }
		
		/**
		 * @brief Get the exponent.
		 * 
		 * \return 
		 */
		phys_float gamma()const { return m_gamma; }

	};
}// namespace agn