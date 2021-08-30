#pragma once

#include <TRV2.h>
#include <memory>
#include "Tile.h"

class GameWorld
{
public:
	GameWorld(int width, int height);

	Tile& GetTile(int x, int y);
	void SetTile(int x, int y, const Tile& tile);


	void RenderWorld(OpenGLSpriteRenderer* renderer, const RectI& renderRect);

	static constexpr int TILE_SIZE = 16;

	ReadonlyProperty(int, Width, width);
	ReadonlyProperty(int, Height, height);
private:
	std::unique_ptr<Tile[]> _tiles;
};