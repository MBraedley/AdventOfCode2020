// Day15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <cassert>

int main()
{
	std::vector<std::uint64_t> input = { 9,12,1,4,17,0,18 };

	std::unordered_map<std::uint64_t, std::pair<std::optional<std::uint64_t>, std::uint64_t>> positions;
	for (std::uint64_t i = 0; i < input.size(); i++)
	{
		positions[input[i]] = std::make_pair<std::optional<std::uint64_t>, std::uint64_t>(std::nullopt, i + 1);
	}

	std::uint64_t numTurns = input.size();
	std::uint64_t lastVal = input.back();
	while (numTurns < 30000000)
	{
		numTurns++;
		if (!positions[lastVal].first.has_value())
		{
			lastVal = 0;
			positions[0].first = positions[0].second;
			positions[0].second = numTurns;
		}
		else
		{
			auto nextVal = positions[lastVal].second - positions[lastVal].first.value();
			if (positions.find(nextVal) != positions.end())
			{
				positions[nextVal].first = positions[nextVal].second;
			}
			else
			{
				positions[nextVal].first = std::nullopt;
			}
			positions[nextVal].second = numTurns;
			lastVal = nextVal;
		}

		if (numTurns == 2020)
		{
			std::cout << lastVal << std::endl;
		}
	}

	std::cout << lastVal << std::endl;
}
