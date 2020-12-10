// Day10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::uint32_t line;
	std::vector<std::uint32_t> adapters;

	adapters.push_back(0); //power source

	while (inputStrm >> line)
	{
		adapters.push_back(line);
	}

	std::sort(adapters.begin(), adapters.end());
	adapters.push_back(adapters.back() + 3);	//adapter within computer

	std::vector<std::uint32_t> diffs;
	for (std::size_t i = 0; i < adapters.size() - 1; i++)
	{
		diffs.push_back(adapters[i + 1] - adapters[i]);
	}

	std::cout << std::count(diffs.begin(), diffs.end(), 1) * std::count(diffs.begin(), diffs.end(), 3) << std::endl;

	int optCount = 0;

	for (std::size_t i = 0; i < diffs.size() - 1; i++)
	{
		if (diffs[i] == 1 && diffs[i + 1] == 1)
		{
			optCount++;
		}
	}

	std::uint64_t options = std::uint64_t(1) << optCount;

	std::vector<std::uint32_t> query = { 1, 1, 1, 1 };

	auto searchStart = diffs.begin();
	auto findIt = std::search(searchStart, diffs.end(), query.begin(), query.end());
	while (findIt != diffs.end())
	{
		options--;
		searchStart = findIt + 1;
		findIt = std::search(searchStart, diffs.end(), query.begin(), query.end());
	}

	std::cout << options <<std::endl;
}
