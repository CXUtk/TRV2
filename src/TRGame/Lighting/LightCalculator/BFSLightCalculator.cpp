#include "BFSLightCalculator.h"

#include <TRGame/TRGame.hpp>
#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/WorldResources.h>
#include <TRGame/Worlds/Tile.h>
#include <TRGame/Worlds/TileSection.h>

BFSLightCalculator::BFSLightCalculator(LightCommon* lightCommon) : _lightCommonData(lightCommon)
{
	
}

BFSLightCalculator::~BFSLightCalculator()
{}

void BFSLightCalculator::AddLight(const Light & light)
{
	_lights.push_back(light);
}

void BFSLightCalculator::ClearLights()
{
	_lights.clear();
}

void BFSLightCalculator::Calculate()
{
	auto common = _lightCommonData;
	const int totalBlocks = common->TileRectWorld.Size.x * common->TileRectWorld.Size.y;

	auto threadPool = TRGame::GetInstance()->GetThreadPool();
	volatile int finishFlag[3] = { 0 };
	for (int i = 0; i < 3; i++)
	{
		threadPool->RunAsync([this, i, &finishFlag]()-> void {
			calculateOneChannel(i);
			finishFlag[i] = 1;
		});
	}

	for (int channel = 0; channel < 3; channel++)
	{
		while (!finishFlag[channel]) {}
		for (int i = 0; i < totalBlocks; i++)
		{
			float x = _luminances[channel][i];
			x = glm::clamp(x, 0.f, 1.f);
			common->TileColors[channel][i] = x;
		}
	}
}

void BFSLightCalculator::calculateOneChannel(int channel)
{
	const auto common = _lightCommonData;
	const int totalBlocks = common->TileRectWorld.Size.x * common->TileRectWorld.Size.y;

	memset(_luminances[channel], 0, sizeof(_luminances[channel][0]) * totalBlocks);
	memset(_visited[channel], 0, sizeof(_visited[channel][0]) * totalBlocks);

	const auto L = _luminances[channel];
	const auto VIS = _visited[channel];
	auto& Q = _lightQueue[channel];

	for (const auto& light : _lights)
	{
		glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);
		if (light.Color[channel] == 0.f || !common->IsTileCoordInRange(lightTile)) continue;
		int curId = common->GetBlockId(lightTile - common->TileRectWorld.Position);
		Q.push(LightNode{ glm::i16vec2(lightTile), light.Color[channel], 0 });
		L[curId] = light.Color[channel];
	}
	while (!Q.empty())
	{
		LightNode node = Q.top();
		Q.pop();

		int curId = common->GetBlockId(glm::ivec2(node.Pos) - common->TileRectWorld.Position);
		if (VIS[curId]) continue;
		VIS[curId] = true;

		if (node.Luminance < L[curId]) continue;

		// if (!canTilePropagateLight(node.Pos)) continue;
		for (int i = 0; i < 8; i++)
		{
			glm::ivec2 nxtPos(node.Pos.x + LightCommon::dX[i], node.Pos.y + LightCommon::dY[i]);
			if (common->IsTileCoordInRange(nxtPos))
			{
				int nxtId = common->GetBlockId(nxtPos - common->TileRectWorld.Position);

				float luminance = calculateLuminance(nxtPos, i, L[curId]);
				if (luminance > LightCommon::MIN_LUMINANCE && L[nxtId] < luminance)
				{
					L[nxtId] = luminance;
					Q.push(LightNode{ glm::i16vec2(nxtPos), L[nxtId] });
				}
			}
		}
	}
}

float BFSLightCalculator::calculateLuminance(glm::ivec2 worldCoord, int dir, float curLuminance)
{
	const auto common = _lightCommonData;

	auto& tileCache = common->GetCachedTile(worldCoord);
	auto& tileData = TRGame::GetInstance()->GetWorldResources()->GetTileObjectData(tileCache.Type);
	float newLumin = curLuminance - LightCommon::distA[dir];
	if (tileData.Solid)
	{
		newLumin -= LightCommon::distA[dir] * 2;
	}
	return newLumin;
}
