// Day15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cassert>

int main()
{
	std::vector<std::uint64_t> input = { 9,12,1,4,17,0,18 };
	input.reserve(2020);

	std::unordered_map<std::uint64_t, std::vector<std::uint64_t>> positions;
	for (std::size_t i = 0; i < input.size(); i++)
	{
		positions[input[i]].push_back(i);
	}

	while (input.size() < 2020)
	{
		if (positions[input.back()].size() == 1)
		{
			positions[0].push_back(input.size());
			input.push_back(0);
		}
		else
		{
			assert(positions[input.back()].size() != 0);
			auto posVec = positions[input.back()];
			auto prevPos = posVec[posVec.size() - 2];
			auto nextVal = input.size() - prevPos - 1;
			positions[nextVal].push_back(input.size());
			input.push_back(nextVal);
		}
	}

	std::cout << input.back() << std::endl;

	while (input.size() < 30000000)
	{
		if (positions[input.back()].size() == 1)
		{
			positions[0].push_back(input.size());
			input.push_back(0);
		}
		else
		{
			assert(positions[input.back()].size() != 0);
			auto posVec = positions[input.back()];
			auto prevPos = posVec[posVec.size() - 2];
			auto nextVal = input.size() - prevPos - 1;
			positions[nextVal].push_back(input.size());
			input.push_back(nextVal);
		}
	}

	std::cout << input.back() << std::endl;
}
