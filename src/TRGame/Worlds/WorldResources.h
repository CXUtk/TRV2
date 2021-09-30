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

	trv2::Texture2D* GetTileTexture(int tileType);
	glm::vec3 GetTileMapColor(int tileType);

	trv2::Texture2D* GetWallTexture(int wallType);
	const WallObjectData& GetWallObjectData(int wallType);

private:
	std::unique_ptr<trv2::Texture2D* []> _tileTextures;
	std::unique_ptr<glm::vec3 []> _tileMapColors;
	std::unique_ptr<bool []> _tileSolid;

	std::unique_ptr<trv2::Texture2D* []> _wallTextures;
	std::unique_ptr<WallObjectData []> _wallObjectData;
};