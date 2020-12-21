#include "FullImage.h"

#include <iostream>
#include <sstream>
#include <regex>
#include <cassert>

FullImage::FullImage(const std::vector<ImageTile>& tiles) :
	m_BareTiles(tiles)
{
	InitializeTileGrid();
	InitializeCompletedImage();
}

std::uint32_t FullImage::GetSeaRoughness()
{
	assert(!m_CompletedImage.empty());
	assert(m_CompletedImage.size() == m_CompletedImage.front().size());
	std::stringstream seaMonsterSstrm;
	seaMonsterSstrm << m_SeaMonster[0] << ".{" << m_CompletedImage.size() - m_SeaMonster[0].size() << "}"
		<< m_SeaMonster[1] << ".{" << m_CompletedImage.size() - m_SeaMonster[0].size() << "}"
		<< m_SeaMonster[2];

	std::regex seaMonsterRegex(seaMonsterSstrm.str());
	std::smatch m;

	auto searchableImages = GetSearchableImages();
	for (auto image : searchableImages)
	{
		std::uint32_t seaMonsterCount = 0;
		std::string searchedImage = image;
		while (std::regex_search(searchedImage, m, seaMonsterRegex))
		{
			std::cout << "Monster Found!" << std::endl;
			seaMonsterCount++;
			searchedImage = searchedImage.substr(m.prefix().length() + 1);
		}

		if (seaMonsterCount > 0)
		{
			return GetTotalActivePixels() - seaMonsterCount * m_NumSeaMonsterPixels;
		}
	}

	return GetTotalActivePixels();
}

std::uint32_t FullImage::GetTotalActivePixels()
{
	std::uint32_t count = 0;
	for (auto row : m_CompletedImage)
	{
		count += std::count(row.begin(), row.end(), '#');
	}

	return count;
}

void FullImage::InitializeTileGrid()
{

	for (std::size_t i = 0; i < m_BareTiles.size() - 1; i++)
	{
		for (std::size_t j = i + 1; j < m_BareTiles.size(); j++)
		{
			m_BareTiles[i].SharesSide(m_BareTiles[j]);
		}
	}
	
	for (const auto& tile : m_BareTiles)
	{
		auto sideCount = tile.GetSharedSideCount();
		assert(sideCount >= 2 && sideCount <= 4);
		if (sideCount == 2)
		{
			m_CornerIds.insert(tile.GetTileId());
		}
	}
}

void FullImage::InitializeCompletedImage()
{
	std::unordered_map<std::uint32_t, ImageTile> tiles;
	for (auto tile : m_BareTiles)
	{
		tiles.emplace(tile.GetTileId(), tile);
	}

	auto firstId = *m_CornerIds.begin();
	ImageTile currentTile = tiles.at(firstId);

	if (currentTile.m_SharedSides.find(ImageTile::ImageSide::TOP) != currentTile.m_SharedSides.end())
	{
		currentTile = FlipVertical(currentTile);
	}

	if (currentTile.m_SharedSides.find(ImageTile::ImageSide::LEFT) != currentTile.m_SharedSides.end())
	{
		currentTile = FlipHorizontal(currentTile);
	}

	m_TileGrid.emplace(std::make_pair<std::uint32_t, std::uint32_t>(0, 0), currentTile);

	bool rightReached = false;

	std::uint32_t x = 1;
	std::uint32_t y = 0;

	while (true)
	{
		auto firstInRow = currentTile;
		rightReached = false;
		while (!rightReached)
		{
			auto nextTile = tiles.at(currentTile.m_SharedSides[ImageTile::ImageSide::RIGHT]);
			while (nextTile.m_SharedSides.find(ImageTile::ImageSide::LEFT) == nextTile.m_SharedSides.end() || nextTile.m_SharedSides[ImageTile::ImageSide::LEFT] != currentTile.GetTileId())
			{
				nextTile = RotateRight(nextTile);
			}

			if (currentTile.m_Sides[ImageTile::ImageSide::RIGHT] != nextTile.m_Sides[ImageTile::ImageSide::LEFT])
			{
				nextTile = FlipVertical(nextTile);
			}

			m_TileGrid.emplace(std::make_pair<std::uint32_t, std::uint32_t>(std::uint32_t(x), std::uint32_t(y)), nextTile);
			x++;
			rightReached = nextTile.m_SharedSides.find(ImageTile::ImageSide::RIGHT) == nextTile.m_SharedSides.end();
			currentTile = nextTile;
		}

		if (firstInRow.m_SharedSides.find(ImageTile::ImageSide::BOTTOM) == firstInRow.m_SharedSides.end())
		{
			break;
		}

		x = 0;
		y++;
		auto nextTile = tiles.at(firstInRow.m_SharedSides[ImageTile::ImageSide::BOTTOM]);
		while (nextTile.m_SharedSides.find(ImageTile::ImageSide::TOP) == nextTile.m_SharedSides.end() || nextTile.m_SharedSides[ImageTile::ImageSide::TOP] != firstInRow.GetTileId())
		{
			nextTile = RotateRight(nextTile);
		}

		if (firstInRow.m_Sides[ImageTile::ImageSide::BOTTOM] != nextTile.m_Sides[ImageTile::ImageSide::TOP])
		{
			nextTile = FlipHorizontal(nextTile);
		}

		m_TileGrid.emplace(std::make_pair(std::uint32_t(x), std::uint32_t(y)), nextTile);
		x++;
		currentTile = nextTile;
	}

	std::uint32_t tileHeight = currentTile.m_ImageData.size();
	std::uint32_t tileWidth = currentTile.m_ImageData.front().size();

	for (std::uint32_t yTile = 0; yTile < 12; yTile++)
	{
		for (std::uint32_t tileRow = 1; tileRow < tileHeight - 1; tileRow++)
		{
			std::string row;
			for (std::uint32_t xTile = 0; xTile < 12; xTile++)
			{
				row += m_TileGrid.at(std::make_pair(std::uint32_t(xTile), std::uint32_t(yTile))).m_ImageData[tileRow].substr(1, tileWidth - 2);
			}
			m_CompletedImage.push_back(row);
		}
	}
}

ImageTile FullImage::FlipHorizontal(ImageTile in)
{
	std::vector<std::string> flippedData;
	for (auto row : in.m_ImageData)
	{
		flippedData.push_back(ImageTile::ReverseString(row));
	}

	ImageTile out(in.m_TileId, flippedData);

	if (in.m_SharedSides.find(ImageTile::ImageSide::TOP) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::TOP] = in.m_SharedSides[ImageTile::ImageSide::TOP];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::BOTTOM) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::BOTTOM] = in.m_SharedSides[ImageTile::ImageSide::BOTTOM];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::RIGHT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::LEFT] = in.m_SharedSides[ImageTile::ImageSide::RIGHT];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::LEFT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::RIGHT] = in.m_SharedSides[ImageTile::ImageSide::LEFT];
	}

	return out;
}

ImageTile FullImage::FlipVertical(ImageTile in)
{
	std::vector<std::string> flippedData(in.m_ImageData.rbegin(), in.m_ImageData.rend());

	ImageTile out(in.m_TileId, flippedData);

	if (in.m_SharedSides.find(ImageTile::ImageSide::TOP) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::BOTTOM] = in.m_SharedSides[ImageTile::ImageSide::TOP];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::BOTTOM) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::TOP] = in.m_SharedSides[ImageTile::ImageSide::BOTTOM];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::RIGHT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::RIGHT] = in.m_SharedSides[ImageTile::ImageSide::RIGHT];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::LEFT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::LEFT] = in.m_SharedSides[ImageTile::ImageSide::LEFT];
	}

	return out;
}

ImageTile FullImage::RotateRight(ImageTile in)
{
	std::vector<std::string> rotatedData;
	rotatedData.resize(in.m_ImageData.front().size());

	for (auto row = in.m_ImageData.rbegin(); row != in.m_ImageData.rend(); row++)
	{
		for (std::size_t j = 0; j < in.m_ImageData.front().size(); j++)
		{
			rotatedData[j].push_back(row->at(j));
		}
	}

	ImageTile out(in.m_TileId, rotatedData);

	if (in.m_SharedSides.find(ImageTile::ImageSide::TOP) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::RIGHT] = in.m_SharedSides[ImageTile::ImageSide::TOP];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::RIGHT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::BOTTOM] = in.m_SharedSides[ImageTile::ImageSide::RIGHT];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::BOTTOM) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::LEFT] = in.m_SharedSides[ImageTile::ImageSide::BOTTOM];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::LEFT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::TOP] = in.m_SharedSides[ImageTile::ImageSide::LEFT];
	}

	return out;
}

ImageTile FullImage::RotateLeft(ImageTile in)
{
	std::vector<std::string> rotatedData;
	rotatedData.resize(in.m_ImageData.front().size());

	for (auto row = in.m_ImageData.begin(); row != in.m_ImageData.end(); row++)
	{
		for (std::size_t j = 0; j < in.m_ImageData.front().size(); j++)
		{
			rotatedData[rotatedData.size() - (j + 1)].push_back(row->at(j));
		}
	}

	ImageTile out(in.m_TileId, rotatedData);

	if (in.m_SharedSides.find(ImageTile::ImageSide::TOP) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::LEFT] = in.m_SharedSides[ImageTile::ImageSide::TOP];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::RIGHT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::TOP] = in.m_SharedSides[ImageTile::ImageSide::RIGHT];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::BOTTOM) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::RIGHT] = in.m_SharedSides[ImageTile::ImageSide::BOTTOM];
	}

	if (in.m_SharedSides.find(ImageTile::ImageSide::LEFT) != in.m_SharedSides.end())
	{
		out.m_SharedSides[ImageTile::ImageSide::BOTTOM] = in.m_SharedSides[ImageTile::ImageSide::LEFT];
	}

	return out;
}

std::vector<std::string> FullImage::GetSearchableImages()
{
	std::vector<std::string> ret;

	std::string searchable;
	// Current orientation
	for (auto row : m_CompletedImage)
	{
		searchable += row;
	}
	ret.push_back(searchable);

	// Rotate 180°
	ret.push_back(ImageTile::ReverseString(searchable));

	// Flip horizontal
	searchable.clear();
	for (auto row : m_CompletedImage)
	{
		searchable += ImageTile::ReverseString(row);
	}
	ret.push_back(searchable);

	// Rotate 180°, covers flip vertical
	ret.push_back(ImageTile::ReverseString(searchable));

	// Rotate by 90° and do the 4 above operations
	std::vector<std::string> rotatedData;
	rotatedData.resize(m_CompletedImage.front().size());

	for (auto row = m_CompletedImage.rbegin(); row != m_CompletedImage.rend(); row++)
	{
		for (std::size_t j = 0; j < m_CompletedImage.front().size(); j++)
		{
			rotatedData[j].push_back(row->at(j));
		}
	}

	// Current orientation
	searchable.clear();
	for (auto row : rotatedData)
	{
		searchable += row;
	}
	ret.push_back(searchable);

	// Rotate 180°
	ret.push_back(ImageTile::ReverseString(searchable));

	// Flip horizontal
	searchable.clear();
	for (auto row : rotatedData)
	{
		searchable += ImageTile::ReverseString(row);
	}
	ret.push_back(searchable);

	// Rotate 180°, covers flip vertical
	ret.push_back(ImageTile::ReverseString(searchable));

	return ret;
}

