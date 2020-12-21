#pragma once
#include "ImageTile.h"

#include <map>
#include <unordered_set>

class FullImage
{
public:
	FullImage(const std::vector<ImageTile>& tiles);

	std::unordered_set<std::uint32_t> GetCornerIds() const { return m_CornerIds; }

	std::uint32_t GetSeaRoughness();

private:
	void InitializeTileGrid();
	void InitializeCompletedImage();

	ImageTile FlipHorizontal(ImageTile in);
	ImageTile FlipVertical(ImageTile in);
	ImageTile RotateRight(ImageTile in);
	ImageTile RotateLeft(ImageTile in);

	std::vector<ImageTile> m_BareTiles;
	std::map<std::pair<std::uint32_t, std::uint32_t>, ImageTile> m_TileGrid;
	std::vector<std::string> m_CompletedImage;

	std::unordered_set<std::uint32_t> m_CornerIds;

	std::vector<std::string> m_SeaMonster =
	{
		"                  # ",
		"#    ##    ##    ###",
		" #  #  #  #  #  #   "
	};
};
