// Day24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <cassert>

struct Pos3D
{
	Pos3D(std::int64_t _x, std::int64_t _y, std::int64_t _z) : x(_x), y(_y), z(_z) {};

	std::int64_t x;
	std::int64_t y;
	std::int64_t z;

	Pos3D& operator+=(const Pos3D& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;

		return *this;
	}

	Pos3D& operator*=(const std::uint32_t& multi)
	{
		this->x *= multi;
		this->y *= multi;
		this->z *= multi;
		
		return *this;
	}
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
			return hx ^ (hy << 1) ^ (hz << 2);
		}
	};
}

bool operator==(const Pos3D& lhs, const Pos3D& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Pos3D& lhs, const Pos3D& rhs)
{
	return !(lhs == rhs);
}

Pos3D operator+(const Pos3D& lhs, const Pos3D& rhs)
{
	return Pos3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

enum class HexGridDirection
{
	NW,
	NE,
	E,
	SE,
	SW,
	W,
};

const std::unordered_map<HexGridDirection, Pos3D> DirectionUnit =
{
	{HexGridDirection::NW, {0, 1, -1}},
	{HexGridDirection::NE, {1, 0, -1}},
	{HexGridDirection::E,  {1, -1, 0}},
	{HexGridDirection::SE, {0, -1, 1}},
	{HexGridDirection::SW, {-1, 0, 1}},
	{HexGridDirection::W,  {-1, 1, 0}},
};

std::unordered_set<Pos3D> GetNeigbours(const Pos3D& pos)
{
	std::unordered_set<Pos3D> ret;

	for (const auto& [dir, offset] : DirectionUnit)
	{
		ret.insert(pos + offset);
	}

	return ret;
}

std::unordered_map<Pos3D, bool> GetNextGen(const std::unordered_map<Pos3D, bool>& currentGen)
{
	std::unordered_set<Pos3D> tilesToCheck;
	for (const auto& [pos, active] : currentGen)
	{
		if (active)
		{
			tilesToCheck.merge(GetNeigbours(pos));
			tilesToCheck.insert(pos);
		}
	}

	std::unordered_map<Pos3D, bool> nextGen;
	for (Pos3D pos : tilesToCheck)
	{
		std::uint32_t count = 0;
		for (const Pos3D& test : GetNeigbours(pos))
		{
			if (auto item = currentGen.find(test); item != currentGen.end() && item->second)
			{
				count++;
			}
		}

		if (auto item = currentGen.find(pos); item != currentGen.end() && item->second)	//if black
		{
			if (!(count == 0 || count > 2))
			{
				nextGen.emplace(pos, true);
			}
		}
		else
		{
			if (count == 2)
			{
				nextGen.emplace(pos, true);
			}
		}
	}

	return nextGen;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::unordered_map<Pos3D, bool> floorMap;

	std::ifstream inputStrm(inputFile);
	std::string line;

	while (std::getline(inputStrm, line))
	{
		Pos3D tile(0, 0, 0);

		bool goNorth = false;
		bool goSouth = false;

		for (char c : line)
		{
			if (c == 'n')
			{
				goNorth = true;
				continue;
			}

			if (c == 's')
			{
				goSouth = true;
				continue;
			}

			HexGridDirection dir;
			if (c == 'e')
			{
				if (goNorth)
				{
					dir = HexGridDirection::NE;
				}
				else if (goSouth)
				{
					dir = HexGridDirection::SE;
				}
				else
				{
					dir = HexGridDirection::E;
				}
			}
			else //if (c == 'w')
			{
				if (goNorth)
				{
					dir = HexGridDirection::NW;
				}
				else if (goSouth)
				{
					dir = HexGridDirection::SW;
				}
				else
				{
					dir = HexGridDirection::W;
				}
			}

			goNorth = false;
			goSouth = false;

			tile += DirectionUnit.at(dir);
		}

		if (floorMap.find(tile) != floorMap.end())
		{
			floorMap[tile] = !floorMap[tile];
		}
		else
		{
			floorMap[tile] = true;
		}
	}

	std::uint32_t count = std::count_if(floorMap.begin(), floorMap.end(), [](const std::pair<Pos3D, bool> iter) -> bool
		{
			return iter.second;
		});

	std::cout << count << std::endl;

	// Part 2
	for (std::uint32_t i = 0; i < 100; i++)
	{
		floorMap = GetNextGen(floorMap);
	}

	count = std::count_if(floorMap.begin(), floorMap.end(), [](const std::pair<Pos3D, bool> iter) -> bool
		{
			return iter.second;
		});

	std::cout << count << std::endl;
}
