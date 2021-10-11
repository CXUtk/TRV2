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
	//auto universalRenderer = Engine::GetInstance()->GetUniversalRenderer();

	//for (auto& triangle : triangles)
	//{
	//	universalRenderer->DrawWiredTriangle(triangle.Pos[0], triangle.Pos[1], triangle.Pos[2]);
	//}

	//universalRenderer->Flush(PrimitiveType::TRIANGLE_LIST, worldProjection);

	//int i = 0;
	//for (auto& segment : drawSegments)
	//{
	//	universalRenderer->DrawLine(segment.Start, segment.End, glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1));
	//}

	//universalRenderer->Flush(PrimitiveType::LINE_LIST, worldProjection);

	//triangles.clear();
	//triangles.push_back(Triangle(glm::vec2(0), glm::vec2(100, 100), glm::vec2(200, 0)));

	//auto graphicsDevice = TRGame::GetInstance()->GetEngine()->GetGraphicsDevice();
	//graphicsDevice->BindVertexArray(_vao);

	//auto shader = assetManager->GetShader("builtin::pure");
	//graphicsDevice->UseShader(shader);

	//shader->SetParameterfm4x4("uWorldTransform", worldProjection);

	//graphicsDevice->SetBufferData(trv2::BufferType::ARRAY_BUFFER, _vbo,
	//	sizeof(Triangle) * triangles.size(), triangles.data(), BufferHint::DYNAMIC_DRAW);

	//graphicsDevice->SetPolygonMode(PolygonMode::WIREFRAME);
	//graphicsDevice->DrawPrimitives(PrimitiveType::TRIANGLE_LIST, triangles.size() * 3, 0);
	//graphicsDevice->SetPolygonMode(PolygonMode::FILL);

	//graphicsDevice->UnbindVertexArray();
}

float Lighting::GetLight(glm::ivec2 coord)
{
	return 0.f;
	//if (coord.x < _tileRect.Position.x || coord.x >= _tileRect.Position.x + _tileRect.Size.x
	//	|| coord.y < _tileRect.Position.y || coord.y >= _tileRect.Position.y + _tileRect.Size.y)
	//{
	//	assert(false);
	//	return 0.f;
	//}
	//auto tileRect = _tileRect;
	//auto getId = [tileRect](glm::ivec2 pos) {
	//	int x = pos.x;
	//	int y = pos.y;
	//	return y * tileRect.Size.x + x;
	//};
	//int id = getId(coord - _tileRect.Position);
	//return glm::smoothstep(0.f, 1.f, 1.f - distArray[id] / MAXDIST);
}
