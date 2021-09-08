#pragma once

#include <TREngine/TREngine_Interfaces.hpp>
#include <memory>
#include "Tile.hpp"

class GameWorld
{
public:
	GameWorld(int width, int height);

	Tile& GetTile(int x, int y);
	void SetTile(int x, int y, const Tile& tile);


	void RenderWorld(trv2::SpriteRenderer* renderer, const trv2::RectI& renderRect);

	static constexpr int TILE_SIZE = 16;

	ReadonlyProperty(int, Width, width);
	ReadonlyProperty(int, Height, height);
private:
	std::unique_ptr<Tile[]> _tiles;
};