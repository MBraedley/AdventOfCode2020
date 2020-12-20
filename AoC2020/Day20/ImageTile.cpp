#include "ImageTile.h"

#include <cassert>

ImageTile::ImageTile(std::uint32_t tileId, std::vector<std::string> imageData) :
	m_TileId(tileId),
	m_ImageData(imageData)
{
	m_Sides.insert(m_ImageData.front());
	m_Sides.insert(m_ImageData.back());

	std::string left, right;
	for (auto row : m_ImageData)
	{
		left.push_back(row.front());
		right.push_back(row.back());
	}

	m_Sides.insert(left);
	m_Sides.insert(right);
}

bool ImageTile::SharesSide(ImageTile& other)
{
	bool ret = false;
	for (auto mySide : m_Sides)
	{
		for (auto theirSide : other.m_Sides)
		{
			if (mySide == theirSide || ReverseString(mySide) == theirSide)
			{
				assert(!ret);
				ret = true;

				assert(m_SharedSides.find(mySide) == m_SharedSides.end());
				m_SharedSides.emplace(mySide, other.GetTileId());

				assert(other.m_SharedSides.find(theirSide) == other.m_SharedSides.end());
				other.m_SharedSides.emplace(theirSide, m_TileId);
			}
		}
	}

	return ret;
}
