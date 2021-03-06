// Day06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <bitset>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::vector<std::vector<std::bitset<26>>> groupAns;
	groupAns.push_back({});

	while (std::getline(inputStrm, line))
	{
		if (!line.empty())
		{
			std::bitset<26> personAns;
			for (char c : line)
			{
				personAns.set(c - 'a');
			}
			groupAns.back().push_back(personAns);
		}
		else
		{
			groupAns.push_back({});
		}
	}

	int part1 = 0;
	int part2 = 0;

	for (auto group : groupAns)
	{
		std::bitset<26> ans1;
		std::bitset<26> ans2;
		ans2.set();
		for (auto person : group)
		{
			ans1 |= person;
			ans2 &= person;
		}

		part1 += ans1.count();
		part2 += ans2.count();
	}

	std::cout << part1 << std::endl << part2 << std::endl;
}
