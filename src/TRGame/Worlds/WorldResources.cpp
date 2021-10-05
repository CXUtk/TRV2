#include "WorldResources.h"

#include <TREngine/Engine.h>
#include <TREngine/Core/Assets/assets.h>

static constexpr int TILE_TYPES = 10;
static constexpr int WALL_TYPES = 2;

WorldResources::WorldResources()
{
	auto assetManager = trv2::Engine::GetInstance()->GetAssetsManager();

	_tileObjectDataArray = std::make_unique<TileObjectData[]>(TILE_TYPES);
	_wallObjectDataArray = std::make_unique<WallObjectData[]>(WALL_TYPES);

	TileObjectData empty{};
	// Empty air
	_tileObjectDataArray[0] = empty;

	// Rock
	TileObjectData rock{};
	rock.Solid = true;
	rock.BlockLight = true;
	rock.Texture = assetManager->GetTexture2D("stone");
	rock.MapColor = glm::vec3(0.4, 0.4, 0.4);
	_tileObjectDataArray[1] = rock;

	// Dirt
	TileObjectData dirt{};
	dirt.Solid = true;
	dirt.BlockLight = true;
	dirt.Texture = assetManager->GetTexture2D("dirt");
	dirt.MapColor = glm::vec3(0.7, 0.4, 0.2);
	_tileObjectDataArray[2] = dirt;


	// Wall
	// Empty wall
	_wallObjectDataArray[0] = WallObjectData();

	// Brick Wall
	WallObjectData brickWallObjectData{};
	brickWallObjectData.UseShader = true;
	brickWallObjectData.Shader = assetManager->GetShader("tex:brick");
	_wallObjectDataArray[1] = brickWallObjectData;

}

WorldResources::~WorldResources()
{}

const TileObjectData& WorldResources::GetTileObjectData(int tileType)
{
	return _tileObjectDataArray[tileType];
}

const WallObjectData& WorldResources::GetWallObjectData(int wallType)
{
	return _wallObjectDataArray[wallType];
}
