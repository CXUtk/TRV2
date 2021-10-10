#include "Lighting.h"
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


static constexpr int dX[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
static constexpr int dY[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };

static constexpr int dX4[4] = { 1, -1, 0, 0 };
static constexpr int dY4[4] = { 0, 0, 1, -1 };
static constexpr float MIN_LUMINANCE = 0.f;
static constexpr float DECREASE = 0.0625;
static constexpr float DECREASE_DIAG = 0.0883;
static constexpr float distA[8] = { DECREASE, DECREASE, DECREASE, DECREASE,
		DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG };

static constexpr float MAXDIST = 16.f;
static constexpr int K = 1;
static constexpr float EPS = 1e-4;


struct KeyPoint;

using KeyPointHandle = int;
std::map<std::pair<int, int>, KeyPointHandle> keyPointsMap;
std::vector<KeyPoint> keyPointList;



struct KeyPointInfo
{
	int SegId;
	bool IsEnd;

	bool operator<(const KeyPointInfo& info) const
	{
		return IsEnd > info.IsEnd;
	}
};

struct KeyPoint
{
	glm::ivec2 Pos;

	std::vector<KeyPointInfo> ConjunctionInfo;

	KeyPoint(glm::ivec2 tilePos) : Pos(tilePos)
	{}


};

struct KeyPointTmp
{
	KeyPointHandle Handle;
	double Angle;

	bool operator<(const KeyPointTmp& p) const
	{
		return Angle < p.Angle;
	}

	glm::vec2 GetWorldPos() const
	{
		return glm::vec2(keyPointList[Handle].Pos * 16);
	}
};

struct Triangle
{
	glm::vec2 Pos[3];
	Triangle(glm::vec2 A, glm::vec2 B, glm::vec2 C)
	{
		Pos[0] = A, Pos[1] = B, Pos[2] = C;
	}
};
struct Ray
{
	glm::vec2 Start, Dir;
	glm::vec2 Eval(float t)
	{
		return Start + Dir * t;
	}
};

struct Segment
{
	glm::ivec2 Start, End;
	KeyPointHandle StartHandle;
	int SegId;
	bool Horizontal;

	Segment(KeyPointHandle start, KeyPointHandle end, int id, bool horizontal) : SegId(id), Horizontal(horizontal)
	{
		StartHandle = start;
		Start = keyPointList[start].Pos * 16;
		End = keyPointList[end].Pos * 16;
	}

	bool IntersectionTest(const Ray& ray, float& t)
	{
		double dv = ray.Dir[Horizontal];
		double travel = Start[Horizontal] - ray.Start[Horizontal];
		if (dv == 0.0)
		{
			float a = Start[!Horizontal] / ray.Dir[!Horizontal];
			float b = End[!Horizontal] / ray.Dir[!Horizontal];
			if (a > b) std::swap(a, b);
			t = a;
			if (a < 0) t = b;
			return t >= 0 && travel == 0.0;
		}
		t = travel / dv;
		if (t < -EPS) return false;

		double other = ray.Start[!Horizontal] + travel * ray.Dir[!Horizontal] / dv;
		double minn = std::min(Start[!Horizontal], End[!Horizontal]);
		double maxx = std::max(Start[!Horizontal], End[!Horizontal]);
		if (other < minn - EPS || other > maxx + EPS) return false;
		return true;
	}
};



static std::vector<Triangle> triangles;

Lighting::Lighting()
{

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
	_lights.clear();
	_directionalLights.clear();
}

void Lighting::AddLight(const Light& light)
{
	_lights.push_back(light);
}

void Lighting::AddDirectionalLight(const Light& light)
{
	_directionalLights.push_back(light);
}

void Lighting::CalculateLight(const trv2::RectI& tileRectCalc, const trv2::RectI& tileRectScreen)
{
	_tileRectScreen = tileRectScreen;

	auto sectionRect = GameWorld::GetTileSectionRect(tileRectCalc);
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
	volatile int finishFlag[3] = { 0 };
	for (int i = 0; i < 3; i++)
	{
		threadPool->RunAsync([this, i, &finishFlag]()-> void {
			calculateOneChannel(_lights, i);
			finishFlag[i] = 1;
		});
	}

	calculateDirectionLight(_directionalLights);

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


static std::vector<Segment> drawSegments;
void Lighting::DrawDirectionalTriangles(const glm::mat4& worldProjection)
{
	auto universalRenderer = Engine::GetInstance()->GetUniversalRenderer();

	int i = 0;
	int sz = triangles.size();
	for (auto& triangle : triangles)
	{
		//if (i == sz - 2)
		//{
		//	universalRenderer->DrawWiredTriangle(triangle.Pos[0], triangle.Pos[1], triangle.Pos[2], glm::vec4(1, 0, 0, 1));
		//}

		//else
		//{
		universalRenderer->DrawWiredTriangle(triangle.Pos[0], triangle.Pos[1], triangle.Pos[2]);
		//}
		i++;
	}
	universalRenderer->Flush(PrimitiveType::TRIANGLE_LIST, worldProjection);

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




static void AddOneSegment(int& id, KeyPointHandle A, KeyPointHandle B, bool horizontal,
	std::vector<Segment>& segments, const glm::vec2& center)
{
	auto& keyA = keyPointList[A];
	auto& keyB = keyPointList[B];

	auto vA = glm::vec2(keyA.Pos * 16) - center;
	auto vB = glm::vec2(keyB.Pos * 16) - center;
	float jud = cross2(vA, vB);

	// jud >= 0 means vB is on the left of vA, starting point should be A
	bool s = (jud >= 0);
	keyA.ConjunctionInfo.push_back(KeyPointInfo{ id, !s });
	keyB.ConjunctionInfo.push_back(KeyPointInfo{ id, s });

	if (s)
	{
		segments.push_back(Segment{ A, B, id, horizontal });
	}
	else
	{
		segments.push_back(Segment{ B, A, id, horizontal });
	}

	id++;
}


static KeyPointHandle GetKeyPointHandle(glm::ivec2 pos)
{
	auto p = keyPointsMap.find(std::pair<int, int>{pos.x, pos.y});
	if (p != keyPointsMap.end())
	{
		return p->second;
	}
	KeyPointHandle handle = keyPointList.size();
	keyPointsMap[std::pair<int, int>{pos.x, pos.y}] = handle;
	keyPointList.push_back(KeyPoint(pos));
	return handle;
}




void Lighting::calculateDirectionLight(const std::vector<Light>& dLights)
{
	for (const auto& light : dLights)
	{
		glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);
		triangles.clear();
		keyPointsMap.clear();
		keyPointList.clear();

		std::vector<Segment> segments{};
		std::vector<KeyPointTmp> keypointTmps{};


		// Add borders, remember 0 - 3 are borders
		auto startPos = lightTile - glm::ivec2(light.Radius);
		auto endPos = lightTile + glm::ivec2(light.Radius + 1);
		trv2::RectI areaRect(startPos, endPos - startPos);
		addShadowSegments(areaRect, segments, light.Position);

		// Limit seraching range, generate computing segments
		for (int y = -light.Radius; y <= light.Radius; y++)
		{
			for (int x = -light.Radius; x <= light.Radius; x++)
			{
				auto curTilePos = lightTile + glm::ivec2(x, y);
				if (getCachedTileType(curTilePos) == 0) continue;
				auto tileRect = trv2::RectI(curTilePos, glm::ivec2(1));

				addShadowSegments(tileRect, segments, light.Position);
			}
		}

		drawSegments = segments;

		// Sort keypoints by their polar angle
		int keyPointSz = keyPointList.size();
		for (int i = 0; i < keyPointSz; i++)
		{
			auto& pt = keyPointList[i];
			if (pt.ConjunctionInfo.empty()) continue;
			std::sort(pt.ConjunctionInfo.begin(), pt.ConjunctionInfo.end());
			auto pos = glm::vec2(pt.Pos * 16) - light.Position;
			keypointTmps.push_back(KeyPointTmp{ i, atan2(pos.y, pos.x) });
		}

		std::sort(keypointTmps.begin(), keypointTmps.end());


		// Calculate initial sweeping line data structure
		glm::vec2 lastKeyPosition{};
		std::deque<int> segmentQueue{};
		std::set<int> activeSegments{};
		std::map<int, int> testSegments;

		// Scan first ray
		Ray firstRay{ light.Position, glm::normalize(keypointTmps[0].GetWorldPos() - light.Position) };
		int totSegments = segments.size();

		std::set<int> startpoints;
		for (int i = 0; i < totSegments; i++)
		{
			auto& segment = segments[i];
			float t;
			if (segment.IntersectionTest(firstRay, t))
			{
				testSegments[segment.SegId]++;
			}
		}
		startpoints.insert(keypointTmps[0].Handle);
		for (auto key : startpoints)
		{
			for (const auto& conj : keyPointList[key].ConjunctionInfo)
			{
				if (conj.IsEnd)
				{
					testSegments[conj.SegId]--;
				}
				else
				{
					testSegments[conj.SegId]++;
				}
			}
		}

		for (const auto& pair : testSegments)
		{
			if (pair.second > 0)
			{
				activeSegments.insert(pair.first);
			}
		}

		float minnTime = std::numeric_limits<float>::infinity();
		int minSeg = -1;
		for (auto id : activeSegments)
		{
			if (id < 4) continue;
			auto& segment = segments[id];
			float t;
			if (segment.IntersectionTest(firstRay, t))
			{
				if (t <= minnTime)
				{
					minnTime = t;
					minSeg = id;
				}
			}
		}
		if (minSeg == -1)
		{
			for (auto id : activeSegments)
			{
				if (id > 4) break;
				auto& segment = segments[id];
				float t;
				if (segment.IntersectionTest(firstRay, t))
				{
					if (t <= minnTime)
					{
						minnTime = t;
						minSeg = id;
					}
				}
			}
		}

		// Add initial nearest wall to sweep line structure
		lastKeyPosition = firstRay.Eval(minnTime);

		//for (auto& segment : segments)
		//{
		//	float t;
		//	if (segment.IntersectionTest(firstRay, t))
		//	{
		//		activeSegments.insert(segment.SegId);
		//		if (t < minnTime)
		//		{
		//			minnTime = t;
		//			minSeg = segment.SegId;
		//		}
		//	}
		//}
		//lastKeyPosition = firstRay.Eval(minnTime);

		int sz = keypointTmps.size();
		int cnt = 0;

		for (int i = 1; i < sz + 1; i++)
		{
			auto& cur = keypointTmps[i % sz];
			auto& nxt = keypointTmps[(i + 1) % sz];
			auto keypointPosCur = cur.GetWorldPos();
			auto keypointPosNext = nxt.GetWorldPos();

			// Batch the points with the same polar angle
			++cnt;
			if (i == sz || std::abs(cross2(keypointPosCur - light.Position, keypointPosNext - light.Position)) > EPS)
			{
				Ray currentRay{ light.Position, glm::normalize(keypointPosCur - light.Position) };
				minnTime = std::numeric_limits<float>::infinity();
				minSeg = -1;

				for (auto id : activeSegments)
				{
					if (id < 4) continue;
					auto& segment = segments[id];
					float t;
					if (segment.IntersectionTest(currentRay, t))
					{
						if (t <= minnTime)
						{
							minnTime = t;
							minSeg = id;
						}
					}
				}
				if (minSeg == -1)
				{
					for (auto id : activeSegments)
					{
						if (id > 4) continue;
						auto& segment = segments[id];
						float t;
						if (segment.IntersectionTest(currentRay, t))
						{
							if (t <= minnTime)
							{
								minnTime = t;
								minSeg = id;
							}
						}
					}
				}

				int oldMinSeg = minSeg;
				float oldMinTime = minnTime;


				assert(activeSegments.count(0) || activeSegments.count(1) || activeSegments.count(2) || activeSegments.count(3));
				testSegments.clear();
				for (int j = i - cnt + 1; j <= i; j++)
				{
					auto keypointHandle = keypointTmps[j % sz].Handle;
					auto& keypoint = keyPointList[keypointHandle];
					auto keypointPos = keypointTmps[j % sz].GetWorldPos();

					for (const auto& conj : keypoint.ConjunctionInfo)
					{
						if (conj.IsEnd)
						{
							testSegments[conj.SegId]--;
						}
						else
						{
							testSegments[conj.SegId]++;
						}
					}
				}
				if (activeSegments.count(58))
				{
					if (true);
				}
				for (const auto& pair : testSegments)
				{
					if (pair.second > 0)
					{
						auto p = activeSegments.find(pair.first);
						if (p == activeSegments.end())
						{
							activeSegments.insert(pair.first);
						}
						else
						{
							if (true);
						}
					}
					else if (pair.second < 0)
					{
						auto p = activeSegments.find(pair.first);
						if (p != activeSegments.end())
						{
							activeSegments.erase(p);
						}
						else
						{
							if (true);
						}
					}
				}
				assert(activeSegments.count(0) || activeSegments.count(1) || activeSegments.count(2) || activeSegments.count(3));

				minnTime = std::numeric_limits<float>::infinity();
				minSeg = -1;
				for (auto id : activeSegments)
				{
					if (id < 4) continue;
					auto& segment = segments[id];
					float t;
					if (segment.IntersectionTest(currentRay, t))
					{
						if (t <= minnTime)
						{
							minnTime = t;
							minSeg = id;
						}
					}
				}
				if (minSeg == -1)
				{
					for (auto id : activeSegments)
					{
						if (id > 4) continue;
						auto& segment = segments[id];
						float t;
						if (segment.IntersectionTest(currentRay, t))
						{
							if (t <= minnTime)
							{
								minnTime = t;
								minSeg = id;
							}
						}
					}
				}
				assert(minSeg != -1);
				//if (i == 1 ||oldMinSeg == -1)
				//{
				//	auto lastPos = currentRay.Eval(oldMinTime);
				//	auto newPos = currentRay.Eval(minnTime);
				//	triangles.push_back(Triangle(lastKeyPosition, light.Position, lastPos));
				//	lastKeyPosition = newPos;
				//	continue;
				//}
				if (i == sz || oldMinSeg != minSeg)
				{
					auto lastPos = currentRay.Eval(oldMinTime);
					auto newPos = currentRay.Eval(minnTime);

					//if (std::abs(cross2(lastKeyPosition - light.Position, lastPos - light.Position)) > 1.f)
					//{
					triangles.push_back(Triangle(lastKeyPosition, light.Position, lastPos));
					//}
					lastKeyPosition = newPos;
				}

				cnt = 0;
			}
		}
		//auto& lastTriangle = triangles.back();
		//triangles.push_back(Triangle(lastTriangle.Pos[2], light.Position, triangles.front().Pos[0]));
	}
}

int Lighting::getCachedTileType(glm::ivec2 worldCoord) const
{
	worldCoord -= _tileRect.Position;
	return _cachedTileTypes[worldCoord.y * _tileRect.Size.x + worldCoord.x];
}

void Lighting::addShadowSegments(const trv2::RectI& worldTileRect, std::vector<Segment>& segments, const glm::vec2& center)
{
	KeyPointHandle BottomLeft = GetKeyPointHandle(worldTileRect.BottomLeft());
	KeyPointHandle TopLeft = GetKeyPointHandle(worldTileRect.TopLeft());
	KeyPointHandle TopRight = GetKeyPointHandle(worldTileRect.TopRight());
	KeyPointHandle BottomRight = GetKeyPointHandle(worldTileRect.BottomRight());

	// Push 4 edges to segments list
	int id = segments.size();

	// If given rect is a tile block, we should only push the profile edge
	if (worldTileRect.Size == glm::ivec2(1))
	{
		auto tileCoord = worldTileRect.BottomLeft();

		auto leftBlock = glm::ivec2(tileCoord.x - 1, tileCoord.y);
		auto upBlock = glm::ivec2(tileCoord.x, tileCoord.y + 1);
		auto rightBlock = glm::ivec2(tileCoord.x + 1, tileCoord.y);
		auto downBlock = glm::ivec2(tileCoord.x, tileCoord.y - 1);

		if (isValidCoord(leftBlock) && getCachedTileType(leftBlock) == 0)
		{
			// Left
			AddOneSegment(id, BottomLeft, TopLeft, false, segments, center);
		}

		if (isValidCoord(upBlock) && getCachedTileType(upBlock) == 0)
		{
			// Up
			AddOneSegment(id, TopLeft, TopRight, true, segments, center);
		}

		if (isValidCoord(rightBlock) && getCachedTileType(rightBlock) == 0)
		{
			// Right
			AddOneSegment(id, TopRight, BottomRight, false, segments, center);
		}

		if (isValidCoord(downBlock) && getCachedTileType(downBlock) == 0)
		{
			// Bottom
			AddOneSegment(id, BottomRight, BottomLeft, true, segments, center);
		}

	}
	else
	{

		// Left
		AddOneSegment(id, BottomLeft, TopLeft, false, segments, center);

		// Up
		AddOneSegment(id, TopLeft, TopRight, true, segments, center);

		// Right
		AddOneSegment(id, TopRight, BottomRight, false, segments, center);

		// Bottom
		AddOneSegment(id, BottomRight, BottomLeft, true, segments, center);
	}
}

bool Lighting::canTilePropagateLight(glm::ivec2 worldCoord)
{
	return true;
}


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

