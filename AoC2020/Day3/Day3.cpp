// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>
#include <utility>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);

	std::string line;
	std::vector<std::string> map;

	while (std::getline(inputStrm, line))
	{
		map.push_back(line);
	}

	std::vector<std::pair<int, int>> modes =
	{
		{1, 1},
		{3, 1},
		{5, 1},
		{7, 1},
		{1, 2}
	};

	std::vector<std::uint64_t> treesHit(modes.size(), 0);

	for (size_t row = 0; row < map.size(); row++)
	{
		std::string mapRow = map[row];
		for (size_t modeId = 0; modeId < modes.size(); modeId++)
		{
			if (row % modes[modeId].second == 0 && mapRow[modes[modeId].first * row / modes[modeId].second % mapRow.size()] == '#')
			{
				treesHit[modeId]++;
			}
		}
	}

	std::uint64_t product = 1;
	for (auto hitCount : treesHit)
	{
		product *= hitCount;
		std::cout << hitCount << std::endl;
	}

	std::cout << product << std::endl;
}
