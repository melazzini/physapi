#include"agn_utils.hpp"
#include<iomanip>
namespace agn
{
	void loadClouds(t_clouds& clouds, const std::string& cloudsFile)
	{
		std::ifstream fin;
		fin.open(cloudsFile);
		if (!fin.good())
		{
			std::cout << "there was a problem while trying to open the file:  "
				<< cloudsFile << std::endl;
			std::cout << "program aborted!" << std::endl;
			abort();
		}

		phys_float x_i, y_i, z_i;
		while (fin >> x_i)
		{
			fin >> y_i;
			fin >> z_i;
			clouds.push_back({ x_i, y_i, z_i });
		}

		fin.clear();
		fin.close();
	}

	void loadHistogram(t_histogram& histogram, const std::string& histogramFile)
	{
		std::ifstream fin;
		fin.open(histogramFile);
		if (!fin.good())
		{
			std::cout << "there was a problem while trying to open the file:  "
				<< histogramFile << std::endl;
			std::cout << "program aborted!" << std::endl;
			abort();
		}

		phys_size NoOfClous;
		phys_float histogramValue;
		phys_float x_i, y_i, z_i;
		while (fin >> NoOfClous)
		{
			fin >> histogramValue;
			histogram[NoOfClous] = histogramValue;
		}

		fin.clear();
		fin.close();
	}

	void printAGNClumpyTorusInfo(phys_float R1,
		phys_float R2,
		phys_float theta0,
		phys_float N_H,
		phys_float N_aver,
		phys_float phi,
		phys_size numOfPhotons,
		phys_size numOfClouds,
		phys_float T_e,
		phys_float R_clouds,
		std::ostream& os)
	{
		os << "   Internal Radius:                        "
			<< std::right << std::setw(5) << R1 << " m"
			<< std::endl;

		os << "   External Radius:                        "
			<< std::right << std::setw(5) << R2 << " m"
			<< std::endl;

		os << "   Half Opening-angle:                     "
			<< std::right << theta0 / 1.0_deg << "    deg"
			<< std::endl;

		os << "   Column Density:                         "
			<< std::right << std::setw(5) << N_H << " m^{-2}"
			<< std::endl;

		os << "   Average Number(sightline) of Clouds:    "
			<< std::right << N_aver
			<< std::endl;

		os << "   Volume Filling Factor:                  "
			<< std::right << std::setw(3) << phi
			<< std::endl;

		os << "   Number of Photons:                      "
			<< std::right << numOfPhotons
			<< std::endl;

		os << "   Number of Clouds:                       "
			<< std::right << numOfClouds
			<< std::endl;

		os << "   Radius of the clouds:                   "
			<< std::right << std::setw(5) << R_clouds << " m"
			<< std::endl;

		os << "   Temperature of Electrons:               "
			<< std::right << std::setw(5)
			<< T_e << " K"
			<< std::endl;
	}
}// namespace agn