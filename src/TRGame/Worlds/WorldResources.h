#pragma once
#include <memory>
#include "Tile.h"

#include <glm/glm.hpp>
#include <TREngine/Core.h>
#include <TRGame/TRGame_Interfaces.h>

class WorldResources
{
public:
	WorldResources();
	~WorldResources();

	const TileObjectData& GetTileObjectData(int tileType);
	const WallObjectData& GetWallObjectData(int wallType);

private:
	std::unique_ptr<TileObjectData []> _tileObjectDataArray;
	std::unique_ptr<WallObjectData []> _wallObjectDataArray;
};