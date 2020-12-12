// Day11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
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

std::map<std::pair<std::int32_t, std::int32_t>, std::set<std::pair<std::int32_t, std::int32_t>>> GetCheckedSeats1(const std::vector<std::string>& map)
{
	std::map<std::pair<std::int32_t, std::int32_t>, std::set<std::pair<std::int32_t, std::int32_t>>> ret;
	auto height = map.size();
	auto width = map.front().size();

	for (std::size_t i = 0; i < height; i++)
	{
		for (std::size_t j = 0; j < width; j++)
		{
			if (map[i][j] != '.')
			{
				std::set<std::pair<std::int32_t, std::int32_t>> s;
				if (i != 0 && j != 0)
					s.insert({ i - 1, j - 1 });

				if (i != 0)
					s.insert({ i - 1, j});

				if (i != 0 && j != width - 1)
					s.insert({ i - 1, j + 1 });

				if (j != 0)
					s.insert({ i, j - 1 });

				if (j != width - 1)
					s.insert({ i, j + 1 });

				if (i != height - 1 && j != 0)
					s.insert({ i + 1, j - 1});

				if (i != height - 1)
					s.insert({ i + 1, j });

				if (i != height - 1 && j != width - 1)
					s.insert({ i + 1, j + 1});

				ret.insert({ {i, j}, s });
			}
		}
	}
	return ret;
}

std::size_t GetNearestNeighbourMultiple(const std::pair<std::int32_t, std::int32_t>& point, const std::pair<std::int32_t, std::int32_t>& direction, const std::vector<std::string>& map)
{
	int height = map.size();
	int width = map.front().size();
	size_t multi = 1;
	while (point.first + multi * direction.first < height
		&& point.first + multi * direction.first >= 0
		&& point.second + multi * direction.second < width
		&& point.second + multi * direction.second >= 0)
	{
		if (map[point.first + multi * direction.first][point.second + multi * direction.second] == 'L')
			return multi;
		multi++;
	}

	return 0;
}

std::map<std::pair<std::int32_t, std::int32_t>, std::set<std::pair<std::int32_t, std::int32_t>>> GetCheckedSeats2(const std::vector<std::string>& map)
{
	std::map<std::pair<std::int32_t, std::int32_t>, std::set<std::pair<std::int32_t, std::int32_t>>> ret;
	auto height = map.size();
	auto width = map.front().size();

	for (std::size_t i = 0; i < height; i++)
	{
		for (std::size_t j = 0; j < width; j++)
		{
			if (map[i][j] != '.')
			{
				if (auto d = GetNearestNeighbourMultiple({ i, j }, { 0, 1 }, map); d != 0)
				{
					ret[{i, j}].insert({ i, j + d });
					ret[{i, j + d}].insert({ i, j });
				}

				if (auto d = GetNearestNeighbourMultiple({ i, j }, { 1, 1 }, map); d != 0)
				{
					ret[{i, j}].insert({ i + d, j + d });
					ret[{i + d, j + d}].insert({ i, j });
				}

				if (auto d = GetNearestNeighbourMultiple({ i, j }, { 1, 0 }, map); d != 0)
				{
					ret[{i, j}].insert({ i + d, j });
					ret[{i + d, j}].insert({ i, j });
				}

				if (auto d = GetNearestNeighbourMultiple({ i, j }, { 1, -1 }, map); d != 0)
				{
					ret[{i, j}].insert({ i + d, j - d });
					ret[{i + d, j - d}].insert({ i, j });
				}
			}
		}
	}
	return ret;
}

void CreateNextGen(const std::vector<std::string>& gen1, std::vector<std::string>& gen2, const std::map<std::pair<std::int32_t, std::int32_t>, std::set<std::pair<std::int32_t, std::int32_t>>>& checkMap, std::int32_t visCount)
{
	for (auto seatArea : checkMap)
	{
		auto seat = seatArea.first;
		int count = 0;
		for (auto check : seatArea.second)
		{
			if (gen1[check.first][check.second] == '#')
				count++;
		}

		if (gen1[seat.first][seat.second] == 'L' && count == 0)
			gen2[seat.first][seat.second] = '#';
		else if (gen1[seat.first][seat.second] == '#' && count >= visCount)
			gen2[seat.first][seat.second] = 'L';
		else
			gen2[seat.first][seat.second] = gen1[seat.first][seat.second];
	}
}

void PrintMap(const std::vector<std::string>& map, std::ostream& strm = std::cout)
{
	for (auto line : map)
	{
		strm << line << std::endl;
	}
	strm << std::endl;
}

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

	std::vector<std::string> lastGen(map.begin(), map.end());
	std::vector<std::string> nextGen(map.begin(), map.end());

	//auto checkMap1 = GetCheckedSeats1(map);

	//do
	//{
	//	lastGen.swap(nextGen);
	//	CreateNextGen(lastGen, nextGen, checkMap1 , 4);
	//	//PrintMap(nextGen);
	//} while (!CompareMaps(lastGen, nextGen));

	//int count1 = 0;
	//for (auto line : nextGen)
	//{
	//	count1 += std::count_if(line.begin(), line.end(), [](char c) { return c == '#'; });
	//}

	//std::cout << count1 << std::endl;

	//Part 2
	//lastGen.clear();
	//nextGen.clear();
	//lastGen.assign(map.begin(), map.end());
	//nextGen.assign(map.begin(), map.end());

	auto checkMap2 = GetCheckedSeats2(map);
	std::ofstream output("output.txt");

	do
	{
		lastGen.swap(nextGen);
		CreateNextGen(lastGen, nextGen, checkMap2, 5);
		PrintMap(nextGen, output);
	} while (!CompareMaps(lastGen, nextGen));

	int count2 = 0;
	for (auto line : nextGen)
	{
		count2 += std::count_if(line.begin(), line.end(), [](char c) { return c == '#'; });
	}

	std::cout << count2 << std::endl;
}
