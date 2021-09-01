#pragma once

#include <TREngine/TREngine_Interfaces.h>
#include <memory>
#include "Tile.h"

class GameWorld
{
public:
	GameWorld(int width, int height);

	Tile& GetTile(int x, int y);
	void SetTile(int x, int y, const Tile& tile);


	void RenderWorld(trv2::ISpriteRenderer& renderer, const trv2::RectI& renderRect);

	static constexpr int TILE_SIZE = 16;

	ReadonlyProperty(int, Width, width);
	ReadonlyProperty(int, Height, height);
private:
	std::unique_ptr<Tile[]> _tiles;
};