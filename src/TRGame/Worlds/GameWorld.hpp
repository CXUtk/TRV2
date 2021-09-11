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

private:
	std::unique_ptr<Tile[]> _tiles;
	std::shared_ptr<trv2::RenderTarget2D> _renderTarget;

	int _tileMaxX, _tileMaxY;

	static constexpr int TILE_SIZE = 16;
};