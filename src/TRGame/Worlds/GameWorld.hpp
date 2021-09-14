#pragma once
#include <memory>

#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>
#include "Tile.hpp"

class GameWorld
{
public:
	GameWorld(int width, int height);

	Tile& GetTile(int x, int y);
	void SetTile(int x, int y, const Tile& tile);

	int GetTileMaxX() const { return _tileMaxX; }
	int GetTileMaxY() const { return _tileMaxY; }

	void RenderWorld(const glm::mat4& projection, trv2::SpriteRenderer* renderer, const trv2::Rect2D<float>& renderRect);


	static constexpr int TILE_SIZE = 16;
	static glm::ivec2 GetLowerWorldCoord(glm::vec2 pos);
	static glm::ivec2 GetUpperWorldCoord(glm::vec2 pos);
private:
	std::unique_ptr<Tile[]> _tiles;
	std::unique_ptr<TileGenLayout[]> _worldGenLayouts;
	std::shared_ptr<trv2::RenderTarget2D> _renderTarget;

	int _tileMaxX, _tileMaxY;
};