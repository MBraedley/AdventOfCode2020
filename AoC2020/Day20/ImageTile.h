#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

class ImageTile
{
	friend class FullImage;
public:
	enum class ImageSide
	{
		TOP,
		RIGHT,
		BOTTOM,
		LEFT,
	};

	ImageTile(std::uint32_t tileId, std::vector<std::string> imageData);
	~ImageTile() = default;

	std::uint32_t GetTileId() const { return m_TileId; }

	bool SharesSide(ImageTile& other);
	std::size_t GetSharedSideCount() const { return m_SharedSides.size(); }

private:
	std::string ReverseString(const std::string& str)
	{
		std::string ret(str.rbegin(), str.rend());
		return ret;
	}

	std::vector<std::string> m_ImageData;
	std::unordered_map<ImageSide, std::string> m_Sides;
	std::uint32_t m_TileId;

	std::unordered_map<ImageSide, std::uint32_t> m_SharedSides;
};
