// Day12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numbers>
#include <filesystem>
#include <cassert>

std::pair<int, int> RotateWaypoint(std::pair<int, int> waypoint, int angle)
{
	if (angle < 0)
		angle += 360;

	std::pair<int, int> ret = waypoint;
	switch (angle)
	{
	case 0:
		return waypoint;
	case 90:
		ret.first = waypoint.second;
		ret.second = -waypoint.first;
		break;
	case 180:
		ret.first = -waypoint.first;
		ret.second = -waypoint.second;
		break;
	case 270:
		ret.first = -waypoint.second;
		ret.second = waypoint.first;
		break;
	default:
		assert(false);
		break;
	}
	return ret;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::vector<std::pair<char, int>> instructions;
	char c;
	int val;
	while (inputStrm >> c >> val)
	{
		instructions.emplace_back(c, val);
	}

	std::pair<int, int> loc1 = std::make_pair<int, int>(0, 0);
	int heading = 1;

	for (auto instruct : instructions)
	{
		if (instruct.first == 'N')
		{
			loc1.first += instruct.second;
		}
		else if (instruct.first == 'S')
		{
			loc1.first -= instruct.second;
		}
		else if (instruct.first == 'E')
		{
			loc1.second += instruct.second;
		}
		else if (instruct.first == 'W')
		{
			loc1.second -= instruct.second;
		}
		else if (instruct.first == 'L')
		{
			heading -= instruct.second / 90;
		}
		else if (instruct.first == 'R')
		{
			heading += instruct.second / 90;
		}
		else if (instruct.first == 'F')
		{
			int norm = heading % 4;
			if (norm < 0)
				norm += 4;
			switch (norm)
			{
			case 0:
				loc1.first += instruct.second;
				break;
			case 1:
				loc1.second += instruct.second;
				break;
			case 2:
				loc1.first -= instruct.second;
				break;
			case 3:
				loc1.second -= instruct.second;
				break;
			default:
				break;
			}
		}
	}

	std::cout << std::abs(loc1.first) + std::abs(loc1.second) << std::endl;

	std::pair<int, int> loc2 = std::make_pair<int, int>(0, 0);
	std::pair<int, int> waypoint = std::make_pair<int, int>(1, 10);


	for (auto instruct : instructions)
	{
		if (instruct.first == 'N')
		{
			waypoint.first += instruct.second;
		}
		else if (instruct.first == 'S')
		{
			waypoint.first -= instruct.second;
		}
		else if (instruct.first == 'E')
		{
			waypoint.second += instruct.second;
		}
		else if (instruct.first == 'W')
		{
			waypoint.second -= instruct.second;
		}
		else if (instruct.first == 'L')
		{
			waypoint = RotateWaypoint(waypoint, instruct.second);
		}
		else if (instruct.first == 'R')
		{
			waypoint = RotateWaypoint(waypoint, -instruct.second);
		}
		else if (instruct.first == 'F')
		{
			loc2.first += instruct.second * waypoint.first;
			loc2.second += instruct.second * waypoint.second;
		}
	}

	std::cout << std::abs(loc2.first) + std::abs(loc2.second) << std::endl;
}
