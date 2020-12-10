// Day05.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::vector<std::uint16_t> seatIds;

	while (std::getline(inputStrm, line))
	{
		std::uint16_t id = 0;
		for (char c : line)
		{
			id = id << 1;
			if (c == 'B' || c == 'R')
			{
				id += 1;
			}
		}
		seatIds.push_back(id);
	}

	std::cout << *std::max_element(seatIds.begin(), seatIds.end()) << std::endl;

	// Part 2
	std::sort(seatIds.begin(), seatIds.end());
	for ( size_t i = 0; i < seatIds.size() - 1; i++)
	{
		if (seatIds[i + 1] - seatIds[i] == 2)
			std::cout << seatIds[i] + 1 << std::endl;
	}
}
