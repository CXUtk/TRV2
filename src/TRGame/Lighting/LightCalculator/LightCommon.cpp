#include "LightCommon.h"

int LightCommon::GetBlockId(glm::ivec2 localCoord) const
{
	assert(localCoord.x >= 0 && localCoord.x < TileRectWorld.Size.x);
	assert(localCoord.y >= 0 && localCoord.y < TileRectWorld.Size.y);
	return localCoord.y * TileRectWorld.Size.x + localCoord.x;
}

bool LightCommon::IsTileCoordInRange(glm::ivec2 worldCoord) const
{
	if (worldCoord.x < TileRectWorld.Position.x || worldCoord.x >= TileRectWorld.Position.x + TileRectWorld.Size.x
		|| worldCoord.y < TileRectWorld.Position.y || worldCoord.y >= TileRectWorld.Position.y + TileRectWorld.Size.y) 
		return false;
	return true;
}

bool LightCommon::CanTilePropagateLight(glm::ivec2 worldCoord) const
{
	return false;
}

const TileCacheInfo& LightCommon::GetCachedTile(glm::ivec2 worldCoord) const
{
	return CachedTile[GetBlockId(worldCoord - TileRectWorld.Position)];
}
