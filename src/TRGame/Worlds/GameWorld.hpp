#pragma once
#include <memory>

#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>
#include "Tile.hpp"

struct BYTE_Color
{
	byte8 R;
	byte8 G;
	byte8 B;
	byte8 A;
};


class GameWorld
{
public:
	GameWorld(int width, int height);

	Tile& GetTile(int x, int y);
	const Tile& GetTile(int x, int y) const;
	void SetTile(int x, int y, const Tile& tile);

	int GetTileMaxX() const { return _tileMaxX; }
	int GetTileMaxY() const { return _tileMaxY; }

	trv2::RectI GetTileRect(const trv2::Rectf& worldRect) const;

	void RenderWorld(const glm::mat4& projection, trv2::SpriteRenderer* renderer, const trv2::Rect2D<float>& renderRect);


	static constexpr int TILE_SIZE = 16;
	static glm::ivec2 GetLowerWorldCoord(glm::vec2 pos);
	static glm::ivec2 GetUpperWorldCoord(glm::vec2 pos);

private:
	std::unique_ptr<Tile[]> _tiles;
	std::unique_ptr<TileGenLayout[]> _worldGenLayouts;

	std::unique_ptr<BYTE_Color[]> _worldMapCache;

	int _tileMaxX, _tileMaxY;
};