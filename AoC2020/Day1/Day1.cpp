// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);

	std::vector<std::int32_t> input;

	std::int32_t value;
	while (inputStrm >> value)
	{
		for (std::uint32_t testVal : input)
		{
			if (value + testVal == 2020)
			{
				std::cout << testVal << "\t" << value << "\t" << testVal * value << std::endl;
			}
		}
		input.push_back(value);
	}

	// Part 2
	for (std::uint32_t i = 0; i < input.size() - 2; i++)
	{
		for (std::uint32_t j = i + 1; j < input.size() - 1; j++)
		{
			for (std::uint32_t k = j + 1; k < input.size(); k++)
			{
				if (input[i] + input[j] + input[k] == 2020)
				{
					std::cout << input[i] << "\t" << input[j] << "\t" << input[k] << "\t" << input[i] * input[j] * input[k] << std::endl;
				}
			}
		}
	}
}
