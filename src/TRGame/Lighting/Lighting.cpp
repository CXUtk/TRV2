#include "Lighting.h"
#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>
#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/Tile.h>

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
	glm::ivec2 Pos;
	float Dist;

	bool operator<(const LightNode& B) const
	{
		return this->Dist > B.Dist;
	}
};

static constexpr int dX[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
static constexpr int dY[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
static constexpr float distA[8] = { 1.f, 1.f, 1.f, 1.f, 1.4142f, 1.4142f, 1.4142f, 1.4142f };
float distArray[VIS_SIZE];
glm::vec3 colorArray[VIS_SIZE];
bool visArray[VIS_SIZE];

constexpr float MAXDIST = 16.f;

static trv2::RectI _tileRect;

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
static int Num = 0;
void Lighting::CalculateLight(trv2::SpriteRenderer* renderer, const glm::mat4& projection, const trv2::RectI& pretileRect)
{
	auto sectionRect = GameWorld::GetTileSectionRect(pretileRect);
	_tileRect = trv2::RectI(sectionRect.Position * GameWorld::TILE_SECTION_SIZE,
		sectionRect.Size * GameWorld::TILE_SECTION_SIZE);

	for (int i = 0; i < _tileRect.Size.x * _tileRect.Size.y; i++)
	{
		colorArray[i] = glm::vec3(0);
	}

	Num = 0;
	for (auto& light : _lights)
	{
		calculateOneLight(light);
	}

	printf("%d\n", Num);
	trv2::BatchSettings setting{};
	renderer->Begin(projection, setting);
	{
		for (int y = pretileRect.Position.y; y < pretileRect.Position.y + pretileRect.Size.y; y++)
		{
			for (int x = pretileRect.Position.x; x < pretileRect.Position.x + pretileRect.Size.x; x++)
			{
				auto coord = glm::ivec2(x, y);
				auto& tile = _gameWorld->GetTile(coord);
				if(tile.IsEmpty())
				{
					/*renderer->Draw(coord, glm::vec2(1), glm::vec2(0),
						0.f, glm::vec4(1));*/
					continue;
				}
				int id = this->getBlockId(coord - _tileRect.Position);
				auto color = colorArray[id];
				if (color == glm::vec3(0)) continue;

				renderer->Draw(coord, glm::vec2(1), glm::vec2(0),
					0.f, glm::vec4(color, 1.f));
			}
		}
	}
	renderer->End();
}

float Lighting::GetLight(glm::ivec2 coord)
{
	if (coord.x < _tileRect.Position.x || coord.x >= _tileRect.Position.x + _tileRect.Size.x
		|| coord.y < _tileRect.Position.y || coord.y >= _tileRect.Position.y + _tileRect.Size.y)
	{
		assert(false);
		return 0.f;
	}
	auto getId = [](glm::ivec2 pos) {
		int x = pos.x;
		int y = pos.y;
		return y * _tileRect.Size.x + x;
	};
	int id = getId(coord - _tileRect.Position);
	return glm::smoothstep(0.f, 1.f, 1.f - distArray[id] / MAXDIST);
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

float Lighting::calculateDistance(glm::ivec2 worldCoord, int dir, float curDist)
{
	auto& tile = _gameWorld->GetTile(worldCoord);
	float newDist = curDist + distA[dir];
	if (tile.Solid)
	{
		if (newDist < 10) newDist = 10;
		newDist += distA[dir];
	}
	return newDist;
}

bool Lighting::canTilePropagateLight(glm::ivec2 worldCoord)
{
	return true;
}


void Lighting::calculateOneLight(const Light& light)
{
	glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);

	// Invalid light
	if (!isValidCoord(lightTile)) return;
	Num++;
	// Reset BFS info in that range
	for (int y = -light.Radius; y <= light.Radius; y++)
	{
		for (int x = -light.Radius; x <= light.Radius; x++)
		{
			auto curTilePos = lightTile + glm::ivec2(x, y);
			if (isValidCoord(curTilePos))
			{
				int id = getBlockId(curTilePos - _tileRect.Position);
				distArray[id] = std::numeric_limits<float>::infinity();
				visArray[id] = false;
			}
		}
	}

	std::priority_queue<LightNode> Q;

	int id = getBlockId(lightTile - _tileRect.Position);
	distArray[id] = 0.f;
	Q.push({ lightTile, 0.f });

	while (!Q.empty())
	{
		LightNode node = Q.top();
		Q.pop();

		int curId = getBlockId(node.Pos - _tileRect.Position);
		if (visArray[curId]) continue;
		visArray[curId] = true;

		if (distArray[curId] >= light.Radius) continue;

		// if (!canTilePropagateLight(node.Pos)) continue;
		for (int i = 0; i < 8; i++)
		{
			glm::ivec2 nxtPos(node.Pos.x + dX[i], node.Pos.y + dY[i]);
			if (isValidCoord(nxtPos))
			{
				int nxtId = getBlockId(nxtPos - _tileRect.Position);
				
				float dist = calculateDistance(nxtPos, i, distArray[curId]);
				if (dist <= light.Radius && distArray[nxtId] > dist)
				{
					distArray[nxtId] = dist;
					Q.push({ nxtPos, distArray[nxtId] });
				}
			}
		}
	}

	auto color = Gamma(light.Color);
	// Reset BFS info in that range
	for (int y = -light.Radius; y <= light.Radius; y++)
	{
		for (int x = -light.Radius; x <= light.Radius; x++)
		{
			glm::ivec2 curTilePos = lightTile + glm::ivec2(x, y);
			if (isValidCoord(curTilePos) )
			{
				int id = getBlockId(curTilePos - _tileRect.Position);
				auto dist = distArray[id];
				if (dist > MAXDIST || glm::isnan(dist) || glm::isinf(dist)) continue;
				auto c = color * glm::mix(0.f, 1.f, 1.f - dist / light.Radius);
				colorArray[id] += c;
			}
		}
	}

}

