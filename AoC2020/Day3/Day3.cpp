// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
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
	std::vector<std::string> map;

	while (std::getline(inputStrm, line))
	{
		map.push_back(line);
	}

	int treesHit = 0;
	size_t xPos = 0;

	for (const std::string& mapRow : map)
	{
		if (mapRow[xPos % mapRow.size()] == '#')
			treesHit++;

		xPos += 3;
	}

	std::cout << treesHit;
}
