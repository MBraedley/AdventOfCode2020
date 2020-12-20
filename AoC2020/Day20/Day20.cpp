// Day20.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ImageTile.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;

	std::vector<ImageTile> tiles;
	while (std::getline(inputStrm, line))
	{
		std::string tile;
		std::uint32_t id;
		std::stringstream sstrm(line);
		sstrm >> tile >> id;

		std::vector<std::string> image;
		while (std::getline(inputStrm, line) && !line.empty())
		{
			image.push_back(line);
		}

		tiles.emplace_back(id, image);
	}

	for (std::size_t i = 0; i < tiles.size() - 1; i++)
	{
		for (std::size_t j = i + 1; j < tiles.size(); j++)
		{
			tiles[i].SharesSide(tiles[j]);
		}
	}

	std::uint64_t product = 1;
	for (const auto& tile : tiles)
	{
		auto sideCount = tile.GetSharedSideCount();
		assert(sideCount >= 2 && sideCount <= 4);
		if (sideCount == 2)
		{
			product *= tile.GetTileId();
		}
	}

	std::cout << product << std::endl;
}
