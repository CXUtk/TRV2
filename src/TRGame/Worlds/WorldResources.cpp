#include "WorldResources.h"

#include <TREngine/Engine.h>
#include <TREngine/Core/Assets/assets.h>

static constexpr int TILE_TYPES = 10;
static constexpr int WALL_TYPES = 2;

WorldResources::WorldResources()
{
	auto assetManager = trv2::Engine::GetInstance()->GetAssetsManager();
	_tileTextures = std::make_unique<trv2::Texture2D* []>(TILE_TYPES);
	_tileMapColors = std::make_unique<glm::vec3[]>(TILE_TYPES);

	_wallTextures = std::make_unique<trv2::Texture2D* []>(WALL_TYPES);
	_wallObjectData = std::make_unique<WallObjectData[]>(WALL_TYPES);


	// Empty air
	_tileTextures[0] = assetManager->GetTexture2D("builtin::sprite");
	_tileMapColors[0] = glm::vec3(0);

	// Rock
	_tileTextures[1] = assetManager->GetTexture2D("stone");
	_tileMapColors[1] = glm::vec3(0.5);


	// Wall
	// Empty wall
	_wallObjectData[0] = WallObjectData();
	_wallTextures[0] = nullptr;

	// Brick Wall
	WallObjectData brickWallObjectData;
	brickWallObjectData.UseShader = true;
	brickWallObjectData.Shader = assetManager->GetShader("tex:brick");
	_wallObjectData[1] = brickWallObjectData;
	_wallTextures[1] = nullptr;

}

WorldResources::~WorldResources()
{}

trv2::Texture2D* WorldResources::GetTileTexture(int tileType)
{
	return _tileTextures[tileType];
}

glm::vec3 WorldResources::GetTileMapColor(int tileType)
{
	return _tileMapColors[tileType];
}

trv2::Texture2D* WorldResources::GetWallTexture(int wallType)
{
	return _wallTextures[wallType];
}

const WallObjectData& WorldResources::GetWallObjectData(int wallType)
{
	return _wallObjectData[wallType];
}
