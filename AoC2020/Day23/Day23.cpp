// Day23.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <mutex>
#include <cassert>

void PrintCups(const std::deque<std::uint32_t>& cups)
{
	for (auto val : cups)
	{
		std::cout << val << " ";
	}
	std::cout << std::endl;
}

void RunGame(std::deque<uint32_t>& cups, const uint32_t& iterations, const uint32_t& maxVal)
{
	for (std::uint32_t i = 0; i < iterations; i++)
	{
		auto head = cups.front();
		cups.pop_front();
		cups.push_back(head);

		std::deque<std::uint32_t> removed;
		while (removed.size() < 3)
		{
			removed.push_back(cups.front());
			cups.pop_front();
		}

		auto dest = head - 1;
		if (dest == 0)
		{
			dest = maxVal;
		}

		while (std::find(removed.begin(), removed.end(), dest) != removed.end())
		{
			dest--;
			if (dest == 0)
			{
				dest = maxVal;
			}
		}

		auto destIter = std::find(cups.rbegin(), cups.rend(), dest).base();

		//destIter++;
		cups.insert(destIter, removed.begin(), removed.end());

		if (i % 10000 == 0)
		{
			std::cout << "Iterartion: " << i << std::endl;
		}
	}
}

int main()
{
	std::deque<std::uint32_t> cups = { 3,6,2,9,8,1,7,5,4 };
	//std::deque<std::uint32_t> cups = { 3,8,9,1,2,5,4,6,7 };

	auto maxVal = *std::max_element(cups.begin(), cups.end());

	RunGame(cups, 100, maxVal);
	PrintCups(cups);

	//Part 2
	//cups = { 3,6,2,9,8,1,7,5,4 };
	//maxVal = 1000000;
	//for (std::uint32_t i = cups.size() + 1; i <= maxVal; i++)
	//{
	//	cups.push_back(i);
	//}

	//RunGame(cups, 10'000'000, maxVal);

	//auto iter = std::find(cups.begin(), cups.end(), 1);
	//std::uint32_t next1 = *(iter + 1);
	//std::uint32_t next2 = *(iter + 2);
	//std::cout << next1 * next2 << std::endl;

	std::vector<std::uint32_t> input = { 3,6,2,9,8,1,7,5,4 };
	input.reserve(1000000);
	for (std::uint32_t i = 10; i <= 1000000; i++)
	{
		input.push_back(i);
	}

	std::vector<std::uint32_t> cupsP2(1000001);
	cupsP2[0] = input[0];
	for (std::uint32_t i = 1; i < input.size(); i++)
	{
		cupsP2[input[i - 1]] = input[i];
	}

	cupsP2[input.size()] = input[0];

	for (std::uint32_t i = 0; i < 10000000; i++)
	{
		std::uint32_t current = cupsP2[0];
		std::uint32_t c1 = cupsP2[current];
		std::uint32_t c2 = cupsP2[c1];
		std::uint32_t c3 = cupsP2[c2];
		std::uint32_t next = cupsP2[c3];

		std::uint32_t dest = current - 1;
		if (dest == 0)
		{
			dest = 1000000;
		}
		while (dest == c1 || dest == c2 || dest == c3)
		{
			dest--;
			if (dest == 0)
			{
				dest = 1000000;
			}
		}

		cupsP2[c3] = cupsP2[dest];
		cupsP2[dest] = c1;
		cupsP2[current] = next;
		cupsP2[0] = next;
	}

	std::uint64_t ans = static_cast<std::uint64_t>(cupsP2[1]) * static_cast<std::uint64_t>(cupsP2[cupsP2[1]]);

	std::cout << ans << std::endl;
}
