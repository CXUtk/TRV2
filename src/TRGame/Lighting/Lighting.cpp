#include "Lighting.h"
#include <TRGame/TRGame.hpp>
#include <TRGame/Player/Player.h>

#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/WorldResources.h>
#include <TRGame/Worlds/Tile.h>
#include <TRGame/Worlds/TileSection.h>

#include <TREngine/Core/Render/SpriteRenderer.h>



Lighting::Lighting()
{}

Lighting::~Lighting()
{}



static constexpr int dX[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
static constexpr int dY[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
static constexpr float MIN_LUMINANCE = 0.f;
static constexpr float DECREASE = 0.0625;
static constexpr float DECREASE_DIAG = 0.0883;
static constexpr float distA[8] = { DECREASE, DECREASE, DECREASE, DECREASE,
		DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG };

static constexpr float MAXDIST = 16.f;
static constexpr int K = 1;


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
	_tileRect = trv2::RectI(sectionRect.Position * GameWorld::TILE_SECTION_SIZE,
		sectionRect.Size * GameWorld::TILE_SECTION_SIZE);

	sectionRect.ForEach([this](glm::ivec2 sectionCoord) {
		const TileSection* section = _gameWorld->GetSection(sectionCoord);
		section->ForEachTile([this](glm::ivec2 coord, const Tile& tile) {
			int id = this->getBlockId(coord - _tileRect.Position);
			_cachedTileTypes[id] = tile.Type;
		});
	});

	auto threadPool = TRGame::GetInstance()->GetThreadPool();
	bool finishFlag[3] = { 0 };
	for (int i = 0; i < 3; i++)
	{
		threadPool->RunAsync([this, i, &finishFlag]()-> void {
			calculateOneChannel(_lights, i);
			finishFlag[i] = true;
		});
	}



	for (int channel = 0; channel < 3; channel++)
	{
		while (!finishFlag[channel]) {}
		for (int i = 0; i < _tileRect.Size.x * _tileRect.Size.y; i++)
		{
			float x = _luminances[channel][i];
			x = glm::clamp(x, 0.f, 1.f);
			_colors[channel][i] = x;
		}
	}
}

void Lighting::DrawLightMap(trv2::SpriteRenderer* renderer, const glm::mat4& projection)
{
	trv2::BatchSettings setting{};
	renderer->Begin(projection, setting);
	{
		_tileRectScreen.ForEach([this, renderer](glm::ivec2 coord) -> void {
			int id = this->getBlockId(coord - _tileRect.Position);
			auto color = glm::vec3(_colors[0][id], _colors[1][id], _colors[2][id]);
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
	auto tileType = getCachedTileType(worldCoord);
	auto& tileData = TRGame::GetInstance()->GetWorldResources()->GetTileObjectData(tileType);
	float newLumin = curLuminance - distA[dir];
	if (tileData.Solid)
	{
		newLumin -= distA[dir] * 2;
	}
	return newLumin;
}

void Lighting::calculateOneChannel(const std::vector<Light>& lights, int channel)
{
	memset(_luminances[channel], 0, sizeof(_luminances[channel][0]) * _tileRect.Size.x * _tileRect.Size.y);
	memset(_visited[channel], 0, sizeof(_visited[channel][0]) * _tileRect.Size.x * _tileRect.Size.y);

	const auto L = _luminances[channel];
	const auto VIS = _visited[channel];
	auto& Q = _lightQ[channel];

	for (const auto& light : lights)
	{
		glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);
		if (light.Color[channel] == 0 || !isValidCoord(lightTile)) continue;
		int curId = getBlockId(lightTile - _tileRect.Position);
		Q.push(LightNode{ glm::i16vec2(lightTile), light.Color[channel], 0 });
		L[curId] = light.Color[channel];
	}
	while (!Q.empty())
	{
		LightNode node = Q.top();
		Q.pop();

		int curId = getBlockId(glm::ivec2(node.Pos) - _tileRect.Position);
		if (VIS[curId]) continue;
		VIS[curId] = true;

		if (node.Luminance < L[curId]) continue;

		// if (!canTilePropagateLight(node.Pos)) continue;
		for (int i = 0; i < 8; i++)
		{
			glm::ivec2 nxtPos(node.Pos.x + dX[i], node.Pos.y + dY[i]);
			if (isValidCoord(nxtPos))
			{
				int nxtId = getBlockId(nxtPos - _tileRect.Position);

				float luminance = calculateLuminance(nxtPos, i, L[curId]);
				if (luminance > MIN_LUMINANCE && L[nxtId] < luminance)
				{
					L[nxtId] = luminance;
					Q.push(LightNode{ glm::i16vec2(nxtPos), L[nxtId] });
				}

			}
		}
	}
}

struct Segment
{
	glm::vec2 Start, End;
	int SideId;
};

struct KeyPoint
{
	glm::vec2 Pos;
	int SideId;
	double _angle;

	KeyPoint(glm::vec2 pos, int id) : Pos(pos), SideId(id)
	{
		_angle = std::atan2((double)Pos.y, (double)Pos.x);
	}

	bool operator<(const KeyPoint& p) const
	{
		return _angle < p._angle;
	}
};

struct Triangle
{
	glm::vec3 Pos[3];
};

static void AddShadowSegments(const trv2::Rectf& rect, std::vector<Segment>& segments, std::vector<KeyPoint>& keypoints)
{
	// Push 4 edges to segments list
	int idLeft = segments.size();
	segments.push_back(Segment{ rect.BottomLeft(), rect.TopLeft(), idLeft });

	int idTop = idLeft + 1;
	segments.push_back(Segment{ rect.TopLeft(), rect.TopRight(), idTop });

	int idRight = idTop + 1;
	segments.push_back(Segment{ rect.TopRight(), rect.BottomRight(), idRight });

	int idBottom = idRight + 1;
	segments.push_back(Segment{ rect.BottomRight(), rect.BottomLeft(), idBottom });


	// Push 8 critical points to the list
	keypoints.push_back(KeyPoint(rect.BottomLeft(), idLeft));
	keypoints.push_back(KeyPoint(rect.BottomLeft(), idBottom));

	keypoints.push_back(KeyPoint(rect.TopLeft(), idLeft));
	keypoints.push_back(KeyPoint(rect.TopLeft(), idTop));

	keypoints.push_back(KeyPoint(rect.TopRight(), idRight));
	keypoints.push_back(KeyPoint(rect.TopRight(), idTop));

	keypoints.push_back(KeyPoint(rect.BottomRight(), idRight));
	keypoints.push_back(KeyPoint(rect.BottomRight(), idBottom));
}

void Lighting::calculateDirectionLight(const std::vector<Light>& dLights)
{
	for (const auto& light : dLights)
	{
		glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);

		// Invalid light
		if (!isValidCoord(lightTile)) return;

		std::vector<Segment> segments;
		std::vector<KeyPoint> keypoints;
		int totSegments = 0;

		auto startPos = (glm::vec2(lightTile) - glm::vec2(light.Radius)) * 16.f;
		trv2::Rectf areaRect(startPos, glm::vec2(light.Radius * 2 + 1) * 16.f);

		AddShadowSegments(areaRect, segments, keypoints);

		// Limit seraching range
		for (int y = -light.Radius; y <= light.Radius; y++)
		{
			for (int x = -light.Radius; x <= light.Radius; x++)
			{
				auto curTilePos = lightTile + glm::ivec2(x, y);
				auto tileRect = trv2::Rectf(glm::vec2(x, y) * 16.f, glm::vec2(16.f));
				
				AddShadowSegments(tileRect, segments, keypoints);
			}
		}

		std::sort(keypoints.begin(), keypoints.end());

		std::vector<Triangle> triangles;
		KeyPoint lastKeyPoint = *keypoints.rbegin();
		std::vector<int> currentSegments;
		
		currentSegments.push_back(lastKeyPoint.SideId);

		for (const auto& keypoint : keypoints)
		{

		}
	}
}

int Lighting::getCachedTileType(glm::ivec2 worldCoord) const
{
	worldCoord -= _tileRect.Position;
	return _cachedTileTypes[worldCoord.y * _tileRect.Size.x + worldCoord.x];
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

