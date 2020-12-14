// Day14.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <regex>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::vector<std::string> input;
	while (std::getline(inputStrm, line))
	{
		input.push_back(line);
	}

	std::regex maskRegex("mask = ([01X]{36})");
	std::regex memRegex("mem\\[(\\d+)\\] = (\\d+)");
	std::smatch m;

	std::bitset<36> onMask;
	std::bitset<36> offMask;
	offMask.set();

	std::unordered_map<std::uint64_t, std::bitset<36>> memory;

	for (std::string instruct : input)
	{
		if (std::regex_match(instruct, m, maskRegex))
		{
			onMask.reset();
			offMask.set();
			std::string newMask = m[1].str();
			std::reverse(newMask.begin(), newMask.end());
			for (std::uint32_t i = 0; i < newMask.size(); i++)
			{
				if (newMask[i] == '0')
				{
					offMask.reset(i);
				}
				else if (newMask[i] == '1')
				{
					onMask.set(i);
				}
				else
				{
					assert(newMask[i] == 'X');
				}
			}
		}
		else if (std::regex_match(instruct, m, memRegex))
		{
			std::bitset<36> val(std::atoi(m[2].str().c_str()));
			memory[std::atoi(m[1].str().c_str())] = val & offMask | onMask;
		}
		else
		{
			assert(false);
		}
	}

	std::uint64_t sum = 0;
	for (auto val : memory)
	{
		sum += val.second.to_ullong();
	}

	std::cout << sum << std::endl;
}
