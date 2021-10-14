#include "DirectionalLightCalculator.h"

#include <algorithm>

#include <TREngine/Core/Utils/Utils.h>
#include <TREngine/Engine.h>
#include <TREngine/Core/Render/render.h>

#include <TRGame/Worlds/WorldResources.h>
#include <TRGame/Worlds/Tile.h>
#include <TRGame/Worlds/TileSection.h>

static std::vector<Edge> drawSegments;

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


void DirectionalLightCalculator::DrawTriangles(const glm::mat4& worldProjection)
{
	auto universalRenderer = trv2::Engine::GetInstance()->GetUniversalRenderer();

	int i = 0;
	int sz = _shadowTriangles.size();
	for (auto& triangle : _shadowTriangles)
	{
		//i++;
		//if(i == 1)
		universalRenderer->DrawWiredTriangle(triangle.Pos[0], triangle.Pos[1], triangle.Pos[2]);
	}
	universalRenderer->SetPolygonMode(trv2::PolygonMode::WIREFRAME);
	universalRenderer->Flush(trv2::PrimitiveType::TRIANGLE_LIST, worldProjection);
	universalRenderer->SetPolygonMode(trv2::PolygonMode::FILL);

	for (auto& segment : drawSegments)
	{
		universalRenderer->DrawLine(segment.Start, segment.End, glm::vec4(0, 0, 1, 1), glm::vec4(1, 0, 0, 1));
	}
	universalRenderer->Flush(trv2::PrimitiveType::LINE_LIST, worldProjection);
}

PVertex DirectionalLightCalculator::getVertexPtr(glm::ivec2 pos)
{
	auto p = _vertexPtrMap.find(std::pair<int, int>{ pos.x, pos.y });
	if (p != _vertexPtrMap.end())
	{
		return p->second;
	}
	int id = _vertices.size();
	_vertices.push_back(Vertex(id, pos));
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
	auto sweepCenter = light.Position;

	glm::ivec2 lightTile = GameWorld::GetLowerWorldCoord(sweepCenter, 0);
	std::vector<KeyPointTmp> keypointTmps{};

	auto startPos = lightTile - glm::ivec2(light.Radius);
	auto endPos = lightTile + glm::ivec2(light.Radius + 1);
	trv2::RectI areaRect(startPos, endPos - startPos);
	addBorderEdges(areaRect, sweepCenter);

	// Limit seraching range, generate computing segments
	for (int y = -light.Radius; y <= light.Radius; y++)
	{
		for (int x = -light.Radius; x <= light.Radius; x++)
		{
			auto curTilePos = lightTile + glm::ivec2(x, y);
			if (common->GetCachedTile(curTilePos).Type == 0) continue;
			auto tileRect = trv2::RectI(curTilePos, glm::ivec2(1));

			addTileEdges(tileRect, sweepCenter);
		}
	}

	drawSegments.assign(_edges.begin(), _edges.end());

	// Sort keypoints by their polar angle
	for (auto& vertex : _vertices)
	{
		PVertex pV = &vertex;
		auto pos = pV->GetWorldPos() - sweepCenter;
		if (pV->ConjunctionInfo.empty()) continue;
		keypointTmps.push_back(KeyPointTmp{ pV, atan2(pos.y, pos.x) });
	}
	std::sort(keypointTmps.begin(), keypointTmps.end());
	
	SweepStructure structure(_edges.size());

	EdgeCmp cmp(structure);

	GeoPQ PQ(cmp);
	structure.EdgeSet = &PQ;

	int startIndex;
	performFirstScan(keypointTmps, structure, sweepCenter, startIndex);

	if (_edges.size() > 100)
	{
		bool a = cmp(&_edges[34], &_edges[16]);
		bool b = cmp(&_edges[16], &_edges[34]);
		if (true);
	}

	int sz = keypointTmps.size();
	int cnt = 0;

	for (int i = startIndex; i < sz + 1; i++)
	{
		auto& cur = keypointTmps[i % sz];
		auto& nxt = keypointTmps[(i + 1) % sz];
		auto keypointPosCur = cur.Vertex->GetWorldPos();
		auto keypointPosNext = nxt.Vertex->GetWorldPos();

		// Batch the points with the same polar angle
		++cnt;
		if (i == sz || std::abs(cross2(keypointPosCur - light.Position,
			keypointPosNext - light.Position)) > LightCommon::EPS)
		{
			performOneScan(keypointTmps, structure, sweepCenter, i - cnt + 1, i);
			cnt = 0;
		}
	}
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

	if (common->IsTileCoordInRange(upBlock) && common->GetCachedTile(upBlock).Type == 0)
	{
		// Up
		addOneSegment(TopLeft, TopRight, true, sweepCenter);
	}

	if (common->IsTileCoordInRange(rightBlock) && common->GetCachedTile(rightBlock).Type == 0)
	{
		// Right
		addOneSegment(TopRight, BottomRight, false, sweepCenter);
	}

	if (common->IsTileCoordInRange(downBlock) && common->GetCachedTile(downBlock).Type == 0)
	{
		// Bottom
		addOneSegment(BottomRight, BottomLeft, true, sweepCenter);
	}
}

void DirectionalLightCalculator::addOneSegment(PVertex A, PVertex B, bool horizontal, glm::vec2 sweepCenter)
{
	auto vA = A->GetWorldPos() - sweepCenter;
	auto vB = B->GetWorldPos() - sweepCenter;
	float jud = cross2(vA, vB);

	// jud >= 0 means vB is on the left of vA, starting point should be A
	bool s;

	int id = _edges.size();
	float d1 = glm::dot(vA, vA), d2 = glm::dot(vB, vB);
	if (jud > 0 || (jud == 0 && d1 < d2))
	{
		_edges.push_back(Edge(A, B, id, horizontal));
		s = true;
	}
	else if(jud < 0 || (jud == 0 && d1 >= d2))
	{
		_edges.push_back(Edge(B, A, id, horizontal));
		s = false;
	}

	PEdge edge = &_edges.back();
	A->ConjunctionInfo.push_back(EndPointInfo{ edge, !s });
	B->ConjunctionInfo.push_back(EndPointInfo{ edge, s });
}

void DirectionalLightCalculator::performFirstScan(const std::vector<KeyPointTmp>& sweep, 
	SweepStructure& structure, glm::vec2 sweepCenter, int& nxtIndex)
{
	if (!sweep.size()) return;
	structure.currentRay = { sweepCenter, sweep[0].Vertex->GetWorldPos() - sweepCenter };
	int totalEdges = _edges.size();

	std::vector<PVertex> startpoints{};
	std::map<int, int> testSegments{};

	for (int i = 0; i < totalEdges; i++)
	{
		auto& edge = _edges[i];
		float t;
		if (edge.IntersectionTest(structure.currentRay, t, true))
		{
			testSegments[edge.Id]++;
			edge.IntersectionTest(structure.currentRay, t, true);
		}
	}

	// Proceed to next different angle
	nxtIndex = 0;
	int sz = sweep.size();
	startpoints.push_back(sweep[0].Vertex);
	
	while (nxtIndex < sz && std::abs(cross2(sweep[(nxtIndex + 1) % sz].Vertex->GetWorldPos() - sweepCenter,
		sweep[nxtIndex].Vertex->GetWorldPos() - sweepCenter)) < LightCommon::EPS)
	{
		nxtIndex++;
		startpoints.push_back(sweep[nxtIndex].Vertex);
	}
	nxtIndex++;

	for (auto startpt : startpoints)
	{
		for (const auto& conj : startpt->ConjunctionInfo)
		{
			if (conj.IsEnd)
			{
				testSegments[conj.Edge->Id]--;
			}
			else
			{
				testSegments[conj.Edge->Id]++;
			}
		}
	}

	for (auto& pair : testSegments)
	{
		if (pair.second < 0)
		{
			eraseEdge(structure, &_edges[pair.first], sweepCenter);
		}
	}
	for (auto& pair : testSegments)
	{
		if (pair.second > 0)
		{
			insertNewEdge(structure, &_edges[pair.first], sweepCenter);
		}
	}


	float minnTime = std::numeric_limits<float>::infinity();
	PEdge minnEdge = nullptr;
	findNearestWall(structure, minnTime, minnEdge);
	assert(minnEdge != nullptr);

	structure.lastKeyPosition = structure.currentRay.Eval(minnTime);
}

void DirectionalLightCalculator::findNearestWall(SweepStructure& structure, 
	float& minnTime, PEdge& minnEdge)
{
	minnTime = std::numeric_limits<float>::infinity();
	minnEdge = nullptr;
	//if (structure.PQ-> > 1)
	//{
	//	if (true);
	//	//printf("%d\n", structure.PQ->top()->Id);
	//}
	if (!structure.EdgeSet->empty())
	{
		for (auto edge : *structure.EdgeSet)
		{
			float t;
			if (edge->IntersectionTest(structure.currentRay, t))
			{
				if (t < minnTime)
				{
					minnTime = t;
					minnEdge = edge;
				}
			}
		}
		//auto p = structure.EdgeSet->rbegin();
		//auto edge = *p;
		//bool has = edge->IntersectionTest(structure.currentRay, minnTime);
		//assert(has);
		//minnEdge = edge;
	}
	if(!minnEdge)
	{
		for (auto edge : structure.borderEdges)
		{
			float t;
			if (edge->IntersectionTest(structure.currentRay, t))
			{
				if (t < minnTime)
				{
					minnTime = t;
					minnEdge = edge;
				}
			}
		}
	}
}

void DirectionalLightCalculator::performOneScan(const std::vector<KeyPointTmp>& sweep,
	SweepStructure& structure, glm::vec2 sweepCenter, int start, int end)
{
	int sz = sweep.size();
	auto currentVertex = sweep[start % sz].Vertex;
	structure.currentRay = { sweepCenter, currentVertex->GetWorldPos() - sweepCenter };

	float minnTime = std::numeric_limits<float>::infinity();
	PEdge minnEdge = nullptr;

	findNearestWall(structure, minnTime, minnEdge);
	assert(minnEdge);

	float oldMinTime = minnTime;
	int oldMinSeg = minnEdge->Id;

	std::map<int, int> testSegments{};
	for (int j = start; j <= end; j++)
	{
		auto vertex = sweep[j % sz].Vertex;

		for (const auto& conj : vertex->ConjunctionInfo)
		{
			if (conj.IsEnd)
			{
				testSegments[conj.Edge->Id]--;
			}
			else
			{
				testSegments[conj.Edge->Id]++;
			}
		}
	}

	for (auto& pair : testSegments)
	{
		if (pair.second < 0)
		{
			eraseEdge(structure, &_edges[pair.first], sweepCenter);
		}
	}
	for (auto& pair : testSegments)
	{
		if (pair.second > 0)
		{
			insertNewEdge(structure, &_edges[pair.first], sweepCenter);
		}
	}

	findNearestWall(structure, minnTime, minnEdge);
	assert(minnEdge);

	if (end == sz || oldMinSeg != minnEdge->Id)
	{
		auto lastPos = structure.currentRay.Eval(oldMinTime);
		auto newPos = structure.currentRay.Eval(minnTime);
		if (_shadowTriangles.size() == 36 - 8)
		{
			if (true);
		}
		//if (std::abs(cross2(structure.lastKeyPosition - sweepCenter, lastPos - sweepCenter)) > 1.f)
		//{
			_shadowTriangles.push_back(Triangle(structure.lastKeyPosition, sweepCenter, lastPos));
		//}
		structure.lastKeyPosition = newPos;
	}
}

void DirectionalLightCalculator::insertNewEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter)
{
	if (edge->IsBorder())
	{
		structure.borderEdges.push_back(edge);
	}
	else
	{
#ifdef _DEBUG
		{
			// test if the wrong edge existed in the state
			for (auto& a : *structure.EdgeSet)
			{
				if (a->Id == edge->Id)
				{
					assert(false);
				}
			}
		}
#endif // DEBUG
		auto p = structure.EdgeSet->find(edge);
		assert(p == structure.EdgeSet->end());
		//structure.activeEdges[edge->Id] = true;
		structure.EdgeSet->insert(edge);
#ifdef _DEBUG
		{
			// test if added the wrong edge
			bool exist = false;
			for (auto& a : *structure.EdgeSet)
			{
				if (a->Id == edge->Id)
				{
					exist = true;
				}
			}
			assert(exist);
		}
#endif // DEBUG
	}
}

void DirectionalLightCalculator::eraseEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter)
{
	if (edge->IsBorder())
	{
		auto p = std::find(structure.borderEdges.begin(), structure.borderEdges.end(), edge);
		structure.borderEdges.erase(p);
	}
	else
	{
		//assert(structure.activeEdges[edge->Id]);
		//structure.activeEdges[edge->Id] = false;
		auto p = structure.EdgeSet->find(edge);
		assert(p != structure.EdgeSet->end());
		structure.EdgeSet->erase(p);
#ifdef _DEBUG
		// test if deleted the wrong edge
		for (auto& a : *structure.EdgeSet)
		{
			if (a->Id == edge->Id)
			{
				assert(false);
			}
		}
#endif // DEBUG

		auto p2 = structure.EdgeSet->find(edge);
		assert(p2 == structure.EdgeSet->end());
	}
}
