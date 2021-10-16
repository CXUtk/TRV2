#include "Lighting.h"
#include "LightCalculator/LightCommon.h"
#include "LightCalculator/BFSLightCalculator.h"
#include "LightCalculator/DirectionalLightCalculator.h"

#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>

#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/WorldResources.h>
#include <TRGame/Worlds/Tile.h>
#include <TRGame/Worlds/TileSection.h>

#include <TREngine/Core/Render/SpriteRenderer.h>
#include <TREngine/Core/Utils/Utils.h>
#include <TREngine/Core/Assets/assets.h>
#include <TREngine/Core/Render/render.h>
#include <TREngine/Engine.h>
#include <set>
using namespace trv2;





Lighting::Lighting()
{
	_lightCommonData = std::make_unique<LightCommon>();
	_bfsCalculator = std::make_unique<BFSLightCalculator>(trv2::ptr(_lightCommonData));
	_directionCalculator = std::make_unique<DirectionalLightCalculator>(trv2::ptr(_lightCommonData));
}

Lighting::~Lighting()
{}

static glm::vec3 Gamma(const glm::vec3 color)
{
	return glm::pow(color, glm::vec3(2.2));
}

static glm::vec3 InvGamma(const glm::vec3 color)
{
	return glm::pow(color, glm::vec3(1 / 2.2));
}

void Lighting::ClearLights()
{
	_bfsCalculator->ClearLights();
	_directionCalculator->ClearLights();
}

void Lighting::AddNormalLight(const Light& light)
{
	_bfsCalculator->AddLight(light);
}

void Lighting::AddDirectionalLight(const Light& light)
{
	_directionCalculator->AddLight(light);
}

void Lighting::CalculateLight(const trv2::RectI& tileRectCalc, const trv2::RectI& tileRectScreen)
{
	_lightCommonData->TileRectScreen = tileRectScreen;
	auto sectionRect = GameWorld::GetTileSectionRect(tileRectCalc);
	_lightCommonData->SectionRect = sectionRect;
	_lightCommonData->TileRectWorld = trv2::RectI(sectionRect.Position * GameWorld::TILE_SECTION_SIZE,
		sectionRect.Size * GameWorld::TILE_SECTION_SIZE);

	auto gameWorld = TRGame::GetInstance()->GetGameWorld();
	_lightCommonData->GameWorld = gameWorld;

	auto common = _lightCommonData.get();
	const int totalBlocks = common->TileRectWorld.Size.x * common->TileRectWorld.Size.y;

	common->SectionRect.ForEach([this, common](glm::ivec2 sectionCoord) {
		const TileSection* section = common->GameWorld->GetSection(sectionCoord);
		section->ForEachTile([this, common](glm::ivec2 coord, const Tile& tile) {
			int id = common->GetBlockId(coord - common->TileRectWorld.Position);
			common->CachedTile[id].Type = tile.Type;
		});
	});

	_bfsCalculator->Calculate();
	_directionCalculator->Calculate();
}



void Lighting::DrawLightMap(trv2::SpriteRenderer* renderer, const glm::mat4& projection)
{
	trv2::BatchSettings setting{};
	renderer->Begin(projection, setting);
	{
		_lightCommonData->TileRectScreen.ForEach([this, renderer](glm::ivec2 coord) -> void {
			auto& common = _lightCommonData;
			int id = common->GetBlockId(coord - common->TileRectWorld.Position);
			
			auto color = glm::vec3(common->TileColors[0][id], 
				common->TileColors[1][id], 
				common->TileColors[2][id]);

			if (color == glm::vec3(0)) return;

			renderer->Draw(coord, glm::vec2(1), glm::vec2(0),
				0.f, glm::vec4(color, 1.f));
		});
	}
	renderer->End();
}



void Lighting::DrawDirectionalTriangles(const glm::mat4& worldProjection)
{
	_directionCalculator->DrawTriangles(worldProjection);
}

float Lighting::GetLight(glm::ivec2 coord)
{
	return 0.f;
}
