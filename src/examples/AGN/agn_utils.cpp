#include"agn_utils.hpp"

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
}// namespace agn