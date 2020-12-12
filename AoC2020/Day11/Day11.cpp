// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <cassert>

bool CompareMaps(const std::vector<std::string>& gen1, const std::vector<std::string>& gen2)
{
	if (gen1.size() != gen2.size())
		return false;

	for (std::size_t i = 0; i < gen1.size(); i++)
	{
		if (gen1[i] != gen2[i])
			return false;
	}
	return true;
}

void CreateNextGen(const std::vector<std::string>& gen1, std::vector<std::string>& gen2)
{
	for (std::size_t i = 1; i < gen1.size() - 1; i++)
	{
		for (std::size_t j = 1; j < gen1[i].size() - 1; j++)
		{
			if (gen1[i][j] != '.')
			{
				int count = 0;
				if (gen1[i - 1][j - 1] == '#') count++;
				if (gen1[i - 1][j] == '#') count++;
				if (gen1[i - 1][j + 1] == '#') count++;
				if (gen1[i][j - 1] == '#') count++;
				if (gen1[i][j + 1] == '#') count++;
				if (gen1[i + 1][j - 1] == '#') count++;
				if (gen1[i + 1][j] == '#') count++;
				if (gen1[i + 1][j + 1] == '#') count++;

				if (gen1[i][j] == 'L' && count == 0)
					gen2[i][j] = '#';
				else if (gen1[i][j] == '#' && count >= 4)
					gen2[i][j] = 'L';
				else
					gen2[i][j] = gen1[i][j];
			}
		}
	}
}

void PrintMap(const std::vector<std::string>& map)
{
	for (auto line : map)
	{
		std::cout << line << std::endl;
	}
	std::cout << std::endl;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::unique_ptr<std::vector<std::string>> map = std::make_unique<std::vector<std::string>>();

	while (std::getline(inputStrm, line))
	{
		line = " " + line + " ";
		map->push_back(line);
	}

	map->push_back(std::string(map->front().size(), ' '));
	map->insert(map->begin(), map->back());

	std::vector<std::string> lastGen(map->begin(), map->end());
	std::vector<std::string> nextGen(map->begin(), map->end());

	do
	{
		lastGen.swap(nextGen);
		CreateNextGen(lastGen, nextGen);
		//PrintMap(nextGen);
	} while (!CompareMaps(lastGen, nextGen));

	int count = 0;
	for (auto line : nextGen)
	{
		count += std::count_if(line.begin(), line.end(), [](char c) { return c == '#'; });
	}

	std::cout << count << std::endl;
}
