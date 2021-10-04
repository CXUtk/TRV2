#include "Lighting.h"
#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/Tile.h>
#include <TRGame/Worlds/WorldResources.h>

#include <algorithm>
#include <vector>
#include <queue>

#include <TREngine/Core/Render/SpriteRenderer.h>

constexpr int VIS_SIZE = 1 << 16;

Lighting::Lighting()
{}

Lighting::~Lighting()
{}


struct LightNode
{
	glm::i16vec2 Pos;
	float Luminance;
	int k;

	bool operator<(const LightNode& B) const
	{
		return this->Luminance < B.Luminance;
	}
};

static constexpr int dX[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
static constexpr int dY[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
static constexpr float MIN_LUMINANCE = 0.f;
static constexpr float DECREASE = 0.0625;
static constexpr float DECREASE_DIAG = 0.0883;
static constexpr float distA[8] = { DECREASE, DECREASE, DECREASE, DECREASE,
		DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG };

static constexpr float MAXDIST = 16.f;
static constexpr int K = 1;

float colors[3][VIS_SIZE];

float luminances[3][VIS_SIZE][K];
bool visited[VIS_SIZE][K];

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
	_lights.clear();
}

void Lighting::AddLight(const Light& light)
{
	_lights.push_back(light);
}

void Lighting::CalculateLight(const trv2::RectI& tileRectScreen)
{
	_tileRectScreen = tileRectScreen;
	auto sectionRect = GameWorld::GetTileSectionRect(_tileRectScreen);

	auto gameWorld = _gameWorld;
	sectionRect.ForEach([gameWorld](glm::ivec2 coord) {
		gameWorld->FlushSectionCache(coord);
	});

	_tileRect = trv2::RectI(sectionRect.Position * GameWorld::TILE_SECTION_SIZE,
		sectionRect.Size * GameWorld::TILE_SECTION_SIZE);

	for (int i = 0; i < 3; i++)
	{
		calculateOneChannel(_lights, i);
	}

	for (int channel = 0; channel < 3; channel++)
	{
		for (int i = 0; i < _tileRect.Size.x * _tileRect.Size.y; i++)
		{
			float x = 0.f;
			for (int k = 0; k < K; k++)
			{
				x += luminances[channel][i][k];
			}
			x = glm::clamp(x, 0.f, 1.f);
			colors[channel][i] = x;
		}
	}
}

void Lighting::DrawLightMap(trv2::SpriteRenderer* renderer, const glm::mat4& projection)
{
	trv2::BatchSettings setting{};
	renderer->Begin(projection, setting);
	{
		_tileRectScreen.ForEach([this, renderer](glm::ivec2 coord) -> void {
			auto& tile = _gameWorld->GetTile(coord);
			int id = this->getBlockId(coord - _tileRect.Position);
			auto color = glm::vec3(colors[0][id], colors[1][id], colors[2][id]);
			if (color == glm::vec3(0)) return;
			renderer->Draw(coord, glm::vec2(1), glm::vec2(0),
				0.f, glm::vec4(color, 1.f));
		});
	}
	renderer->End();
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

int Lighting::getBlockId(glm::ivec2 localCoord)
{
	assert(localCoord.x >= 0 && localCoord.x < _tileRect.Size.x);
	assert(localCoord.y >= 0 && localCoord.y < _tileRect.Size.y);
	return localCoord.y * _tileRect.Size.x + localCoord.x;
}

bool Lighting::isValidCoord(glm::ivec2 worldCoord)
{
	if (worldCoord.x < _tileRect.Position.x || worldCoord.x >= _tileRect.Position.x + _tileRect.Size.x
		|| worldCoord.y < _tileRect.Position.y || worldCoord.y >= _tileRect.Position.y + _tileRect.Size.y) return false;
	return true;
}

bool Lighting::isValidCoordCached(glm::ivec2 worldCoord)
{
	if (worldCoord.x < _tileRect.Position.x || worldCoord.x >= _tileRect.Position.x + _tileRect.Size.x
		|| worldCoord.y < _tileRect.Position.y || worldCoord.y >= _tileRect.Position.y + _tileRect.Size.y) return false;
	return _gameWorld->TileExists(worldCoord);
}

float Lighting::calculateLuminance(glm::ivec2 worldCoord, int dir, float curLuminance)
{
	auto& tile = _gameWorld->GetTile(worldCoord);
	auto& tileData = TRGame::GetInstance()->GetWorldResources()->GetTileObjectData(tile.Type);
	float newLumin = curLuminance - distA[dir];
	if (tileData.Solid)
	{
		newLumin -= distA[dir] * 2;
	}
	return newLumin;
}

void Lighting::calculateOneChannel(const std::vector<Light>& lights, int channel)
{
	memset(luminances[channel], 0, sizeof(float) * _tileRect.Size.x * _tileRect.Size.y);
	memset(visited, 0, sizeof(bool) * _tileRect.Size.x * _tileRect.Size.y);
	std::priority_queue<LightNode> Q;
	for (const auto& light : lights)
	{
		glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);
		if (light.Color[channel] == 0 || !isValidCoord(lightTile)) continue;
		int curId = getBlockId(lightTile - _tileRect.Position);
		Q.push(LightNode{ glm::i16vec2(lightTile), light.Color[channel], 0 });
		luminances[channel][curId][0] = light.Color[channel];
	}
	while (!Q.empty())
	{
		LightNode node = Q.top();
		Q.pop();

		int curId = getBlockId(glm::ivec2(node.Pos) - _tileRect.Position);
		if (visited[curId][node.k]) continue;
		visited[curId][node.k] = true;

		if (node.Luminance < luminances[channel][curId][K - 1]) continue;

		// if (!canTilePropagateLight(node.Pos)) continue;
		for (int i = 0; i < 8; i++)
		{
			glm::ivec2 nxtPos(node.Pos.x + dX[i], node.Pos.y + dY[i]);
			if (isValidCoord(nxtPos))
			{
				int nxtId = getBlockId(nxtPos - _tileRect.Position);

				float luminance = calculateLuminance(nxtPos, i, luminances[channel][curId][node.k]);
				for (int k = 0; k < K; k++)
				{
					if (luminance > MIN_LUMINANCE && luminances[channel][nxtId][k] < luminance)
					{
						luminances[channel][nxtId][k] = luminance;
						Q.push(LightNode{ glm::i16vec2(nxtPos), luminances[channel][nxtId][k], k });
						break;
					}
				}
			}
		}
	}
}

bool Lighting::canTilePropagateLight(glm::ivec2 worldCoord)
{
	return true;
}


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

