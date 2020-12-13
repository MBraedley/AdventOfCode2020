// Day13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	int ts;
	std::string line;

	inputStrm >> ts;
	inputStrm >> line;

	std::vector<int> busLines;

	std::regex busR(R"(x|\d+)");
	std::smatch m;
	while (std::regex_search(line, m, busR))
	{
		busLines.push_back(std::atoi(m[0].str().c_str()));
		line = m.suffix();
	}

	int bestBus = 0;
	int bestTime = std::numeric_limits<int>::max();

	for (auto b : busLines)
	{
		if (b != 0)
		{
			int timeWaiting = b - ts % b;
			if (timeWaiting < bestTime)
			{
				bestTime = timeWaiting;
				bestBus = b;
			}
		}
	}

	std::cout << bestBus * bestTime << std::endl;

	//Part 2
	std::vector<std::pair<std::uint64_t, std::uint64_t>> busRemainders;
	for (std::size_t i = 0; i < busLines.size(); i++)
	{
		if (busLines[i] != 0)
		{
			busRemainders.push_back({ busLines[i], i });
		}
	}

	std::sort(busRemainders.rbegin(), busRemainders.rend());

	std::uint64_t result = busRemainders.front().first - busRemainders.front().second;
	std::uint64_t n = 1;
	for (std::uint64_t i = 0; i < busRemainders.size() - 1; i++)
	{
		auto prev = busRemainders[i];
		auto next = busRemainders[i + 1];
		while ((result + next.second) % next.first != 0)
		{
			result += n * prev.first;
		}
		assert((result + prev.second) % prev.first == 0);
		n *= prev.first;
	}

	std::cout << result << std::endl;
}
