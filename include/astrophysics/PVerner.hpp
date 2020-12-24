#pragma once

#include "astrophysics_utils.hpp"
#include <iostream>
#include "verner_utils.hpp"

namespace physapi
{
	/**
	 * @brief This class is a functor for calculating the photonionization
	 * cross section.
	 *
	 * This class is based on
	 * <a href="https://ui.adsabs.harvard.edu/abs/1996ApJ...465..487V/abstract"> Verner </a>.
	 *
	 * @ingroup astrophysics
	 *
	 * **Example:**
	 *
	 * @code{.cpp}
		#include "PVerner.hpp"
		#include <fstream>

		using namespace std;
		using namespace physapi;

		int main()
		{
			PVerner verner;

			ofstream fout;

			fout.open("photoionization.txt");

			for (phys_float energy_i = 1.0_eV; energy_i < 300.0E3_eV; energy_i += 1.0_eV)
			{
				fout << energy_i / 1.0_eV << "    " << verner(energy_i) << endl;
			}
		}
	 * @endcode
	 *
	 *
	 * output:
	 *
	 *
	 * <a href="photoionization.txt"> photonionization.txt </a>
	 *
	 * @image html photoionization.png
	 *
	 */
	class PVerner
	{

	private:
		// abundance table
		const PAbundanceTable& m_abundances;

		// path to table 1
		//std::string m_table1File;

		// path to table 2
		//std::string m_table2File;

		// table 1 see @keystable1 @valuestable1
		const PVernerTable1& m_table1;

		// table 1 see @keystable2 @valuestable2
		const PVernerTable2& m_table2;

		//this variable holds the value
		//of the photoionization cross
		//section
		phys_float m_sigma;

	public:
		/**
		 * @brief Construct a new PVerner object
		 *
		 * @param table1 path to table 1
		 * @param table2 path to table 2
		 */
		//explicit PVerner(const std::string& table1 = DEFAULT_FILE_PATH_TABLE1,
		//	const std::string& table2 = DEFAULT_FILE_PATH_TABLE2)
		//	: m_abundances{},
		//	m_table1File{ table1 },
		//	m_table2File{ table2 },
		//	m_table1{ m_abundances },
		//	m_table2{ m_abundances }
		PVerner(const PAbundanceTable& abundances,
			const PVernerTable1& table1,
			const PVernerTable2& table2)
			:m_abundances{abundances}, m_table1{table1},m_table2{table2}
		{
			//VernerTable1Hnd config;
			//PProcessTable<t_nz_ne_is, t_table1Value, TABLE1_COLS, VernerLoadTable1> handler1{ m_table1File, config };

			//VernerSelectorTable1 f;

			//handler1(m_table1, f, m_abundances);

			//// loadFromFile<TABLE1_COLS>(m_table1File, m_table1);

			//loadFromFile<TABLE2_COLS>(m_table2File, m_table2);
		}

		//PVerner

		/**
		 * @brief Destroy the PVerner object
		 *
		 */
		virtual ~PVerner() {}

		/**
		 * @brief Get the photonionization cross section
		 *
		 * @param energy photon energy
		 * @param absorbingElements list of all absorbing elements for that energy
		 * @return phys_float photonionization cross section
		 */
		phys_float operator()(phys_float energy,
			std::vector<std::array<phys_float, ABSORBINGELEMENT_PROPERTIES>>& absorbingElements);

		phys_float operator()(phys_float energy);

	private:
		phys_float getSigma(const PVernerTable1::const_iterator& it_Table1, phys_float e);

	private:
		//other internally required parameters:
		//required variables to implement the formula,
		//used in the algorithm, these variables
		//only hold temporary values in the calculation

		phys_float p1{};
		phys_float y{};
		phys_float q{};
		phys_float a{};
		phys_float b{};
		phys_float sigma{};
		phys_float x{};
		phys_float z{};

		//these symbols are used in
		//Prof Verner's algorithm,
		//the symbols denote current
		//atomic number, number
		//of electrons and shell
		//number
		phys_size nz, ne, is;

		//we need also for our implementation
		//a helper array to select from m_Table2
		//the current element
		t_nz_ne CurrentElementTable2;

		//out most-shell number
		phys_size nout{};

		//closest internal-shell number
		phys_size nint{};

		//Ionization energy of the
		//closest internal shell
		phys_float einn{};

		//the next arrays are helper tables
		//of quantum numbers

		//number of the orbital quantum number
		//corresponding to the number of the shell
		const std::array<phys_size, 7> l{
			0, 0, 1, 0, 1, 2, 0 };

		//closest inner-shell corresponding to the number
		//of electrons
		const std::array<phys_size, 30> ninn{
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3,
			3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

		//outermost shell of an atom corresponding to the number
		//of electrons
		const std::array<phys_size, 30> ntot{
			1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4,
			5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7 };
	};
} // namespace physapi
