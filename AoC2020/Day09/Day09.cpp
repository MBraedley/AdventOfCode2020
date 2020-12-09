// Day09.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cassert>

bool SumInSet(const std::deque<std::uint64_t>& buffer, const std::uint64_t testVal)
{
	for (std::size_t i = 0; i < buffer.size() - 1; i++)
	{
		for (std::size_t j = i + 1; j < buffer.size(); j++)
		{
			if (buffer[i] + buffer[j] == testVal)
			{
				return true;
			}
		}
	}
	return false;
}

std::deque<std::uint64_t> FindSumBlock(const std::vector<std::uint64_t>& fullList, const std::uint64_t testVal)
{
	std::deque<std::uint64_t> ret;
	std::uint64_t sum = 0;

	for (auto val : fullList)
	{
		ret.push_back(val);
		sum += val;

		while (sum > testVal)
		{
			sum -= ret.front();
			ret.pop_front();
		}

		if (sum == testVal)
			return ret;
	}

	assert(false);
	return ret;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::vector<std::uint64_t> fullList;
	std::uint64_t num;
	while (inputStrm >> num)
	{
		fullList.push_back(num);
	}

	std::deque<std::uint64_t> buffer(fullList.begin(), fullList.begin() + 25);

	std::deque<std::uint64_t> testList(fullList.begin() + 25, fullList.end());

	while (!testList.empty())
	{
		if (!SumInSet(buffer, testList.front()))
		{
			std::cout << testList.front() << std::endl;
			break;
		}
		buffer.pop_front();
		buffer.push_back(testList.front());
		testList.pop_front();
	}

	const std::uint64_t testVal = testList.front();

	auto range = FindSumBlock(fullList, testVal);
	assert(range.size() >= 2);
	std::uint64_t min = *std::min_element(range.begin(), range.end());
	std::uint64_t max = *std::max_element(range.begin(), range.end());
	std::cout << min + max << std::endl;
}