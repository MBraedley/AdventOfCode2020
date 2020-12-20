#include "ImageTile.h"

#include <cassert>

ImageTile::ImageTile(std::uint32_t tileId, std::vector<std::string> imageData) :
	m_TileId(tileId),
	m_ImageData(imageData)
{
	m_Sides.emplace(ImageSide::TOP, m_ImageData.front());
	m_Sides.emplace(ImageSide::BOTTOM, m_ImageData.back());

	std::string left, right;
	for (auto row : m_ImageData)
	{
		left.push_back(row.front());
		right.push_back(row.back());
	}

	m_Sides.emplace(ImageSide::LEFT, left);
	m_Sides.emplace(ImageSide::RIGHT, right);
}

bool ImageTile::SharesSide(ImageTile& other)
{
	bool ret = false;
	for (auto mySide : m_Sides)
	{
		for (auto theirSide : other.m_Sides)
		{
			if (mySide.second == theirSide.second || ReverseString(mySide.second) == theirSide.second)
			{
				assert(!ret);
				ret = true;

				assert(m_SharedSides.find(mySide.first) == m_SharedSides.end());
				m_SharedSides.emplace(mySide.first, other.GetTileId());

				assert(other.m_SharedSides.find(theirSide.first) == other.m_SharedSides.end());
				other.m_SharedSides.emplace(theirSide.first, m_TileId);
			}
		}
	}

	return ret;
}
