// Day17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <cassert>

struct Pos3D
{
	Pos3D(std::int64_t _x, std::int64_t _y, std::int64_t _z, std::int64_t _w) : x(_x), y(_y), z(_z), w(_w) {};

	std::int64_t x;
	std::int64_t y;
	std::int64_t z;
	std::int64_t w;
};

namespace std
{
	template<> struct hash<Pos3D>
	{
		std::size_t operator()(Pos3D const& pos) const noexcept
		{
			std::size_t hx = std::hash<std::uint64_t>{}(pos.x);
			std::size_t hy = std::hash<std::uint64_t>{}(pos.y);
			std::size_t hz = std::hash<std::uint64_t>{}(pos.z);
			std::size_t hw = std::hash<std::uint64_t>{}(pos.w);
			return hx ^ (hy << 1) ^ (hz << 2) ^ (hw << 3);
		}
	};
}

bool operator==(const Pos3D& lhs, const Pos3D rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

bool operator!=(const Pos3D& lhs, const Pos3D rhs)
{
	return !(lhs == rhs);
}

std::unordered_set<Pos3D> GetNeigbours(const Pos3D& pos)
{
	std::unordered_set<Pos3D> ret;
	for (std::int64_t x : { pos.x - 1, pos.x, pos.x + 1})
	{
		for (std::int64_t y : { pos.y - 1, pos.y, pos.y + 1})
		{
			for (std::int64_t z : { pos.z - 1, pos.z, pos.z + 1})
			{
				for (std::int64_t w : {pos.w - 1, pos.w, pos.w + 1})
				{
					ret.emplace(x, y, z, w);
				}
			}
		}
	}

	return ret;
}

std::unordered_set<Pos3D> GetTestPositions(const std::unordered_set<Pos3D>& map)
{
	std::unordered_set<Pos3D> ret;
	for (const auto& pos : map)
	{
		ret.merge(GetNeigbours(pos));
	}
	return ret;
}

std::uint32_t CountNeighbours(const Pos3D& pos, const std::unordered_set<Pos3D>& map)
{
	std::uint32_t count = 0;
	for (const auto& testPos : GetNeigbours(pos))
	{
		if (testPos != pos && map.find(testPos) != map.end())
		{
			count++;
		}
	}
	return count;
}

bool TestPosition(const Pos3D& pos, const std::unordered_set<Pos3D>& map)
{
	std::uint32_t count = CountNeighbours(pos, map);
	if (map.find(pos) != map.end())	//Currently alive
	{
		return count == 2 || count == 3;
	}
	else
	{
		return count == 3;
	}
}

void GetNextGen(const std::unordered_set<Pos3D>& map, std::unordered_set<Pos3D>& nextGen)
{
	nextGen.clear();
	auto posToTest = GetTestPositions(map);
	for (const auto& testPos : posToTest)
	{
		if (TestPosition(testPos, map))
		{
			nextGen.insert(testPos);
		}
	}
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::vector<std::string> initialMap;

	std::unordered_set<Pos3D> map;

	while (std::getline(inputStrm, line))
	{
		initialMap.push_back(line);
	}

	for (std::size_t i = 0; i < initialMap.size(); i++)
	{
		for (std::size_t j = 0; j < initialMap[i].size(); j++)
		{
			if (initialMap[i][j] == '#')
			{
				map.emplace(j, i, 0, 0);
			}
		}
	}

	std::unordered_set<Pos3D> nextGen;
	for (int cycles = 0; cycles < 6; cycles++)
	{
		GetNextGen(map, nextGen);
		map.swap(nextGen);
	}

	std::cout << map.size() << std::endl;
}