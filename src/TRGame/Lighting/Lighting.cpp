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

//int Lighting::getBlockId(glm::ivec2 localCoord)
//{
//	assert(localCoord.x >= 0 && localCoord.x < _tileRect.Size.x);
//	assert(localCoord.y >= 0 && localCoord.y < _tileRect.Size.y);
//	return localCoord.y * _tileRect.Size.x + localCoord.x;
//}

//bool Lighting::isValidCoord(glm::ivec2 worldCoord)
//{
//	if (worldCoord.x < _tileRect.Position.x || worldCoord.x >= _tileRect.Position.x + _tileRect.Size.x
//		|| worldCoord.y < _tileRect.Position.y || worldCoord.y >= _tileRect.Position.y + _tileRect.Size.y) return false;
//	return true;
//}
//
//bool Lighting::isValidCoordCached(glm::ivec2 worldCoord)
//{
//	if (worldCoord.x < _tileRect.Position.x || worldCoord.x >= _tileRect.Position.x + _tileRect.Size.x
//		|| worldCoord.y < _tileRect.Position.y || worldCoord.y >= _tileRect.Position.y + _tileRect.Size.y) return false;
//	return _gameWorld->TileExists(worldCoord);
//}
//
//float Lighting::calculateLuminance(glm::ivec2 worldCoord, int dir, float curLuminance)
//{
//	auto tileType = getCachedTileType(worldCoord);
//	auto& tileData = TRGame::GetInstance()->GetWorldResources()->GetTileObjectData(tileType);
//	float newLumin = curLuminance - distA[dir];
//	if (tileData.Solid)
//	{
//		newLumin -= distA[dir] * 2;
//	}
//	return newLumin;
//}
//
//void Lighting::calculateOneChannel(const std::vector<Light>& lights, int channel)
//{
//	
//}


//for (int i = 0; i < segments.size(); i++)
//{
//	currentSegments.push_back(i);
//}

//glm::vec2 lastPos;

//for (int i = 0; i < 100 + 1; i++)
//{
//	float r = ((i + 1)%100) / 100.f * 6.28;
//	glm::vec2 dir = glm::vec2(std::cos(r), std::sin(r));
//	Ray currentRay = { glm::vec2(0), dir };
//	float minnTime = std::numeric_limits<float>::infinity();
//	for (auto seg : currentSegments)
//	{
//		auto& segment = segments[seg];
//		float t;
//		if (segment.IntersectionTest(currentRay, t))
//		{
//			if (t < minnTime)
//			{
//				minnTime = t;
//			}
//		}
//	}

//	if (i > 0)
//	{
//		triangles.push_back(Triangle(light.Position + lastPos, light.Position,
//			light.Position + currentRay.Eval(minnTime)));
//	}
//	lastPos = currentRay.Eval(minnTime);
//}


//	int lastClosestSeg = keypoints[0].SideId;
//	int sz = keypoints.size();
//	for (int i = 1; i < sz + 1; i++)
//	{
//		auto& keypoint = keypoints[i % sz];
//		// Before adding each key point, detect the closest wall intersection point
//		// Construct prev - origin - intersection triangle

//		glm::vec2 dir = glm::normalize(keypoint.Pos);
//		Ray currentRay = { glm::vec2(0), dir };
//		auto p = std::find(currentSegments.begin(), currentSegments.end(), keypoint.SideId);
//		if (!keypoint.IsEnd || p == currentSegments.end())
//		{
//			float minnTime = std::numeric_limits<float>::infinity();
//			int minnSeg = -1;
//			for (auto seg : currentSegments)
//			{
//				auto& segment = segments[seg];
//				float t;
//				if (segment.IntersectionTest(currentRay, t))
//				{
//					if (t < minnTime)
//					{
//						minnTime = t;
//						minnSeg = seg;
//					}
//				}
//			}

//			currentSegments.push_back(keypoint.SideId);
//			if (minnTime == std::numeric_limits<float>::infinity()) continue;
//			lastKeyPoint = currentRay.Eval(minnTime);

//			auto& curSegment = segments[keypoint.SideId];
//			float curT;
//			if (curSegment.IntersectionTest(currentRay, curT))
//			{
//				if (curT < minnTime)
//				{
//					/*auto nxtPoint = currentRay.Eval(minnTime);
//					triangles.push_back(Triangle(light.Position + lastKeyPoint, light.Position, light.Position + nxtPoint));*/
//					lastClosestSeg = keypoint.SideId;
//					//lastKeyPoint = keypoint.Pos;
//				}
//			}
//		}
//		else
//		{
//			//float minnTime = std::numeric_limits<float>::infinity();
//			//int minnSeg = -1;
//			//for (auto seg : currentSegments)
//			//{
//			//	auto& segment = segments[seg];
//			//	float t;
//			//	if (segment.IntersectionTest(currentRay, t))
//			//	{
//			//		if (t < minnTime)
//			//		{
//			//			minnTime = t;
//			//			minnSeg = seg;
//			//		}
//			//	}
//			//}
//			if (lastClosestSeg == keypoint.SideId)
//			{
//				triangles.push_back(Triangle(light.Position + lastKeyPoint, light.Position, light.Position + keypoint.Pos));
//			}
//			currentSegments.erase(p);
//			float minnTime = std::numeric_limits<float>::infinity();
//			float minnSeg = -1;
//			for (auto seg : currentSegments)
//			{
//				auto& segment = segments[seg];
//				float t;
//				if (segment.IntersectionTest(currentRay, t))
//				{
//					if (t < minnTime)
//					{
//						minnTime = t;
//						minnSeg = seg;
//					}
//				}
//			}
//			if (minnTime == std::numeric_limits<float>::infinity()) continue;
//			lastKeyPoint = currentRay.Eval(minnTime);
//			lastClosestSeg = minnSeg;
//		}
//	}


//void Lighting::calculateOneLight(const Light& light)
//{
//	glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);
//
//	// Invalid light
//	if (!isValidCoord(lightTile)) return;
//	Num++;
//	// Reset BFS info in that range
//	for (int y = -light.Radius; y <= light.Radius; y++)
//	{
//		for (int x = -light.Radius; x <= light.Radius; x++)
//		{
//			auto curTilePos = lightTile + glm::ivec2(x, y);
//			if (isValidCoord(curTilePos))
//			{
//				int id = getBlockId(curTilePos - _tileRect.Position);
//				distArray[id] = std::numeric_limits<float>::infinity();
//				visArray[id] = false;
//			}
//		}
//	}
//
//
//
//	int id = getBlockId(lightTile - _tileRect.Position);
//	distArray[id] = 0.f;
//	Q.push({ lightTile, 0.f });
//
//	while (!Q.empty())
//	{
//		LightNode node = Q.top();
//		Q.pop();
//
//		int curId = getBlockId(node.Pos - _tileRect.Position);
//		if (visArray[curId]) continue;
//		visArray[curId] = true;
//
//		if (distArray[curId] >= light.Radius) continue;
//
//		// if (!canTilePropagateLight(node.Pos)) continue;
//		for (int i = 0; i < 8; i++)
//		{
//			glm::ivec2 nxtPos(node.Pos.x + dX[i], node.Pos.y + dY[i]);
//			if (isValidCoord(nxtPos))
//			{
//				int nxtId = getBlockId(nxtPos - _tileRect.Position);
//				
//				float dist = calculateDistance(nxtPos, i, distArray[curId]);
//				if (dist <= light.Radius && distArray[nxtId] > dist)
//				{
//					distArray[nxtId] = dist;
//					Q.push({ nxtPos, distArray[nxtId] });
//				}
//			}
//		}
//	}
//
//	auto color = Gamma(light.Color);
//	// Reset BFS info in that range
//	for (int y = -light.Radius; y <= light.Radius; y++)
//	{
//		for (int x = -light.Radius; x <= light.Radius; x++)
//		{
//			glm::ivec2 curTilePos = lightTile + glm::ivec2(x, y);
//			if (isValidCoord(curTilePos) )
//			{
//				int id = getBlockId(curTilePos - _tileRect.Position);
//				auto dist = distArray[id];
//				if (dist > MAXDIST || glm::isnan(dist) || glm::isinf(dist)) continue;
//				auto c = color * glm::mix(0.f, 1.f, 1.f - dist / light.Radius);
//				colorArray[id] += c;
//			}
//		}
//	}
//
//}

