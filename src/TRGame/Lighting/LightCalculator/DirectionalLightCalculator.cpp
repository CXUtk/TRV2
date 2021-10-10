#include "DirectionalLightCalculator.h"

#include <algorithm>
#include <set>

#include <TREngine/Core/Utils/Utils.h>

#include <TRGame/Worlds/GameWorld.h>
#include <TRGame/Worlds/WorldResources.h>
#include <TRGame/Worlds/Tile.h>
#include <TRGame/Worlds/TileSection.h>



struct KeyPointTmp
{
	PVertex Vertex;
	double Angle;

	bool operator<(const KeyPointTmp& p) const
	{
		return Angle < p.Angle;
	}

	glm::vec2 GetWorldPos() const
	{
		return glm::vec2(Vertex->Pos * 16);
	}
};


struct SweepStructure
{
	glm::vec2 lastKeyPosition{};
	std::set<PEdge> activeSegments{};
};



//std::vector<Triangle> triangles;
//static std::vector<Segment> drawSegments;


//
//static void AddOneSegment(int& id, KeyPointHandle A, KeyPointHandle B, bool horizontal,
//	std::vector<Segment>& segments, const glm::vec2& center)
//{
//	auto& keyA = keyPointList[A];
//	auto& keyB = keyPointList[B];
//
//	auto vA = glm::vec2(keyA.Pos * 16) - center;
//	auto vB = glm::vec2(keyB.Pos * 16) - center;
//	float jud = cross2(vA, vB);
//
//	if (jud == 0) return;
//
//	// jud >= 0 means vB is on the left of vA, starting point should be A
//	bool s = (jud >= 0);
//	keyA.ConjunctionInfo.push_back(KeyPointInfo{ id, !s });
//	keyB.ConjunctionInfo.push_back(KeyPointInfo{ id, s });
//
//	if (s)
//	{
//		segments.push_back(Segment{ A, B, id, horizontal });
//	}
//	else
//	{
//		segments.push_back(Segment{ B, A, id, horizontal });
//	}
//
//	id++;
//}
//
//
//static KeyPointHandle GetKeyPointHandle(glm::ivec2 pos)
//{
//	auto p = keyPointsMap.find(std::pair<int, int>{pos.x, pos.y});
//	if (p != keyPointsMap.end())
//	{
//		return p->second;
//	}
//	KeyPointHandle handle = keyPointList.size();
//	keyPointsMap[std::pair<int, int>{pos.x, pos.y}] = handle;
//	keyPointList.push_back(KeyPoint(pos));
//	return handle;
//}
//
//
//
//
//void Lighting::calculateDirectionLight(const std::vector<Light>& dLights)
//{
//	for (const auto& light : dLights)
//	{
//		glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);
//		triangles.clear();
//		keyPointsMap.clear();
//		keyPointList.clear();
//
//		std::vector<Segment> segments{};
//		std::vector<KeyPointTmp> keypointTmps{};
//		int totSegments = 0;
//
//
//		auto startPos = lightTile - glm::ivec2(light.Radius);
//		auto endPos = lightTile + glm::ivec2(light.Radius + 1);
//		trv2::RectI areaRect(startPos, endPos - startPos);
//		addShadowSegments(areaRect, segments, light.Position);
//
//		// Limit seraching range, generate computing segments
//		for (int y = -light.Radius; y <= light.Radius; y++)
//		{
//			for (int x = -light.Radius; x <= light.Radius; x++)
//			{
//				auto curTilePos = lightTile + glm::ivec2(x, y);
//				if (getCachedTileType(curTilePos) == 0) continue;
//				auto tileRect = trv2::RectI(curTilePos, glm::ivec2(1));
//
//				addShadowSegments(tileRect, segments, light.Position);
//			}
//		}
//
//		drawSegments = segments;
//
//		// Sort keypoints by their polar angle
//		int keyPointSz = keyPointList.size();
//		for (int i = 0; i < keyPointSz; i++)
//		{
//			auto& pt = keyPointList[i];
//			std::sort(pt.ConjunctionInfo.begin(), pt.ConjunctionInfo.end());
//			auto pos = glm::vec2(pt.Pos) * 16.f - light.Position;
//			keypointTmps.push_back(KeyPointTmp{ i, atan2(pos.y, pos.x) });
//		}
//
//		std::sort(keypointTmps.begin(), keypointTmps.end());
//
//
//		// Calculate initial sweeping line data structure
//		glm::vec2 lastKeyPosition{};
//		std::set<int> activeSegments{};
//
//		Ray currentRay{ light.Position, glm::normalize(keypointTmps[0].GetWorldPos() - light.Position) };
//		float minnTime = std::numeric_limits<float>::infinity();
//		int minSeg = -1;
//		//for (auto& segment : segments)
//		//{
//		//	float t;
//		//	if (segment.IntersectionTest(currentRay, t))
//		//	{
//		//		activeSegments.insert(segment.SegId);
//		//		if (t < minnTime)
//		//		{
//		//			minnTime = t;
//		//			minSeg = segment.SegId;
//		//		}
//		//	}
//		//}
//		//lastKeyPosition = currentRay.Eval(minnTime);
//
//
//		int oldMinSeg = -1;
//		int sz = keypointTmps.size();
//		for (int i = 0; i < sz + 1; i++)
//		{
//			auto keypointHandle = keypointTmps[i % sz].Handle;
//			auto& keypoint = keyPointList[keypointHandle];
//			auto keypointPos = keypointTmps[i % sz].GetWorldPos();
//
//
//			Ray currentRay{ light.Position, glm::normalize(keypointPos - light.Position) };
//
//			minnTime = std::numeric_limits<float>::infinity();
//			minSeg = -1;
//
//			if (keypoint.ConjunctionInfo.size() == 1 && keypoint.Pos.x == 23 && keypoint.Pos.y == 48)
//			{
//				if (true);
//			}
//
//			if (i == 0)
//			{
//				for (auto& segment : segments)
//				{
//					float t;
//					if (segment.IntersectionTest(currentRay, t))
//					{
//						activeSegments.insert(segment.SegId);
//						if (t <= minnTime)
//						{
//							minnTime = t;
//							minSeg = segment.SegId;
//						}
//					}
//				}
//			}
//			else
//			{
//				for (auto id : activeSegments)
//				{
//					auto& segment = segments[id];
//					float t;
//					if (segment.IntersectionTest(currentRay, t))
//					{
//						if (t <= minnTime)
//						{
//							minnTime = t;
//							minSeg = id;
//						}
//					}
//				}
//				oldMinSeg = minSeg;
//			}
//
//			float oldMinTime = minnTime;
//
//
//
//			for (const auto& conj : keypoint.ConjunctionInfo)
//			{
//				auto p = activeSegments.find(conj.SegId);
//				if (conj.IsEnd)
//				{
//					if (p != activeSegments.end())
//					{
//						activeSegments.erase(p);
//					}
//					else
//					{
//						if (true);
//					}
//				}
//				else
//				{
//					if (p != activeSegments.end())
//					{
//						if (true);
//						//assert(false);
//					}
//					else
//					{
//						activeSegments.insert(conj.SegId);
//					}
//				}
//			}
//
//			minnTime = std::numeric_limits<float>::infinity();
//			minSeg = -1;
//			for (auto id : activeSegments)
//			{
//				auto& segment = segments[id];
//				float t;
//				if (segment.IntersectionTest(currentRay, t))
//				{
//					if (t <= minnTime)
//					{
//						minnTime = t;
//						minSeg = id;
//					}
//				}
//			}
//
//			if (oldMinSeg == -1 || minSeg == -1)
//			{
//				assert(i == 0);
//				auto newPos = currentRay.Eval(minnTime);
//				lastKeyPosition = newPos;
//				oldMinSeg = minSeg;
//				continue;
//			}
//			if (minSeg != oldMinSeg)
//			{
//				auto lastPos = currentRay.Eval(oldMinTime);
//				auto newPos = currentRay.Eval(minnTime);
//
//
//				triangles.push_back(Triangle(lastKeyPosition, light.Position, lastPos));
//				lastKeyPosition = newPos;
//				oldMinSeg = minSeg;
//			}
//		}
//
//		//auto& lastTriangle = triangles.back();
//		//triangles.push_back(Triangle(lastTriangle.Pos[2], light.Position, triangles.front().Pos[0]));
//	}
//}
//
////int Lighting::getCachedTileType(glm::ivec2 worldCoord) const
////{
////	worldCoord -= _tileRect.Position;
////	return _cachedTileTypes[worldCoord.y * _tileRect.Size.x + worldCoord.x];
////}
//
//void Lighting::addShadowSegments(const trv2::RectI& worldTileRect, std::vector<Segment>& segments, const glm::vec2& center)
//{
//	KeyPointHandle BottomLeft = GetKeyPointHandle(worldTileRect.BottomLeft());
//	KeyPointHandle TopLeft = GetKeyPointHandle(worldTileRect.TopLeft());
//	KeyPointHandle TopRight = GetKeyPointHandle(worldTileRect.TopRight());
//	KeyPointHandle BottomRight = GetKeyPointHandle(worldTileRect.BottomRight());
//
//	// Push 4 edges to segments list
//	int id = segments.size();
//
//	// If given rect is a tile block
//	if (worldTileRect.Size == glm::ivec2(1))
//	{
//		auto tileCoord = worldTileRect.BottomLeft();
//
//		auto leftBlock = glm::ivec2(tileCoord.x - 1, tileCoord.y);
//		auto upBlock = glm::ivec2(tileCoord.x, tileCoord.y + 1);
//		auto rightBlock = glm::ivec2(tileCoord.x + 1, tileCoord.y);
//		auto downBlock = glm::ivec2(tileCoord.x, tileCoord.y - 1);
//
//		if (isValidCoord(leftBlock) && getCachedTileType(leftBlock) == 0)
//		{
//			// Left
//			AddOneSegment(id, BottomLeft, TopLeft, false, segments, center);
//		}
//
//		if (isValidCoord(upBlock) && getCachedTileType(upBlock) == 0)
//		{
//			// Up
//			AddOneSegment(id, TopLeft, TopRight, true, segments, center);
//		}
//
//		if (isValidCoord(rightBlock) && getCachedTileType(rightBlock) == 0)
//		{
//			// Right
//			AddOneSegment(id, TopRight, BottomRight, false, segments, center);
//		}
//
//		if (isValidCoord(downBlock) && getCachedTileType(downBlock) == 0)
//		{
//			// Bottom
//			AddOneSegment(id, BottomRight, BottomLeft, true, segments, center);
//		}
//
//	}
//	else
//	{
//
//		// Left
//		AddOneSegment(id, BottomLeft, TopLeft, false, segments, center);
//
//		// Up
//		AddOneSegment(id, TopLeft, TopRight, true, segments, center);
//
//		// Right
//		AddOneSegment(id, TopRight, BottomRight, false, segments, center);
//
//		// Bottom
//		AddOneSegment(id, BottomRight, BottomLeft, true, segments, center);
//	}
//}
//


DirectionalLightCalculator::DirectionalLightCalculator(LightCommon* lightCommon) : _lightCommonData(lightCommon)
{}

DirectionalLightCalculator::~DirectionalLightCalculator()
{}

void DirectionalLightCalculator::AddLight(const Light & light)
{
	_lights.push_back(light);
}

void DirectionalLightCalculator::ClearLights()
{
	_lights.clear();
}

void DirectionalLightCalculator::Calculate()
{
	_shadowTriangles.clear();

	for (auto& light : _lights)
	{
		calculateTrianglesForOneLight(light);
	}
}

PVertex DirectionalLightCalculator::getVertexPtr(glm::ivec2 pos)
{
	auto p = _vertexPtrMap.find(std::pair<int, int>{ pos.x, pos.y });
	if (p != _vertexPtrMap.end())
	{
		return p->second;
	}
	_vertices.push_back(Vertex(pos));
	PVertex vertex = &_vertices.back();
	_vertexPtrMap[std::pair<int, int>{pos.x, pos.y}] = vertex;
	return vertex;
}

void DirectionalLightCalculator::calculateTrianglesForOneLight(const Light& light)
{
	_vertices.clear();
	_edges.clear();
	_vertexPtrMap.clear();

	auto common = _lightCommonData;

	glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(light.Position, 0);
	std::vector<KeyPointTmp> keypointTmps{};
	int totSegments = 0;

	auto startPos = lightTile - glm::ivec2(light.Radius);
	auto endPos = lightTile + glm::ivec2(light.Radius + 1);
	trv2::RectI areaRect(startPos, endPos - startPos);
	addBorderEdges(areaRect, light.Position);

	// Limit seraching range, generate computing segments
	for (int y = -light.Radius; y <= light.Radius; y++)
	{
		for (int x = -light.Radius; x <= light.Radius; x++)
		{
			auto curTilePos = lightTile + glm::ivec2(x, y);
			if (common->GetCachedTile(curTilePos).Type == 0) continue;
			auto tileRect = trv2::RectI(curTilePos, glm::ivec2(1));

			addTileEdges(tileRect, light.Position);
		}
	}

	// Sort keypoints by their polar angle
	for (auto& vertex : _vertices)
	{
		PVertex pV = &vertex;
		auto pos = glm::vec2(pV->Pos * 16) - light.Position;
		keypointTmps.push_back(KeyPointTmp{ pV, atan2(pos.y, pos.x) });
	}
	std::sort(keypointTmps.begin(), keypointTmps.end());

	
	SweepStructure structure;

}

void DirectionalLightCalculator::addBorderEdges(const trv2::RectI& rect, glm::vec2 sweepCenter)
{
	PVertex BottomLeft = getVertexPtr(rect.BottomLeft());
	PVertex TopLeft = getVertexPtr(rect.TopLeft());
	PVertex TopRight = getVertexPtr(rect.TopRight());
	PVertex BottomRight = getVertexPtr(rect.BottomRight());

	// Left
	addOneSegment(BottomLeft, TopLeft, false, sweepCenter);

	// Up
	addOneSegment(TopLeft, TopRight, true, sweepCenter);

	// Right
	addOneSegment(TopRight, BottomRight, false, sweepCenter);

	// Bottom
	addOneSegment(BottomRight, BottomLeft, true, sweepCenter);
}

void DirectionalLightCalculator::addTileEdges(const trv2::RectI& rect, glm::vec2 sweepCenter)
{
	PVertex BottomLeft = getVertexPtr(rect.BottomLeft());
	PVertex TopLeft = getVertexPtr(rect.TopLeft());
	PVertex TopRight = getVertexPtr(rect.TopRight());
	PVertex BottomRight = getVertexPtr(rect.BottomRight());
	auto tileCoord = rect.BottomLeft();

	auto leftBlock = glm::ivec2(tileCoord.x - 1, tileCoord.y);
	auto upBlock = glm::ivec2(tileCoord.x, tileCoord.y + 1);
	auto rightBlock = glm::ivec2(tileCoord.x + 1, tileCoord.y);
	auto downBlock = glm::ivec2(tileCoord.x, tileCoord.y - 1);

	auto common = _lightCommonData;

	if (common->IsTileCoordInRange(leftBlock) && common->GetCachedTile(leftBlock).Type == 0)
	{
		// Left
		addOneSegment(BottomLeft, TopLeft, false, sweepCenter);
	}

	if (common->IsTileCoordInRange(upBlock) && common->GetCachedTile(leftBlock).Type == 0)
	{
		// Up
		addOneSegment(TopLeft, TopRight, true, sweepCenter);
	}

	if (common->IsTileCoordInRange(rightBlock) && common->GetCachedTile(leftBlock).Type == 0)
	{
		// Right
		addOneSegment(TopRight, BottomRight, false, sweepCenter);
	}

	if (common->IsTileCoordInRange(downBlock) && common->GetCachedTile(leftBlock).Type == 0)
	{
		// Bottom
		addOneSegment(BottomRight, BottomLeft, true, sweepCenter);
	}
}

void DirectionalLightCalculator::addOneSegment(PVertex A, PVertex B, bool horizontal, glm::vec2 sweepCenter)
{
	auto vA = glm::vec2(A->Pos * 16) - sweepCenter;
	auto vB = glm::vec2(B->Pos * 16) - sweepCenter;
	float jud = cross2(vA, vB);

	if (jud == 0) return;

	// jud >= 0 means vB is on the left of vA, starting point should be A
	bool s = (jud >= 0);

	if (s)
	{
		_edges.push_back(Edge(A, B, horizontal));
	}
	else
	{
		_edges.push_back(Edge(B, A, horizontal));
	}

	PEdge edge = &_edges.back();
	A->ConjunctionInfo.push_back(EndPointInfo{ edge, !s });
	B->ConjunctionInfo.push_back(EndPointInfo{ edge, s });
}
