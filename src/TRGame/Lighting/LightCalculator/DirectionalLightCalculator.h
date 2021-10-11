#pragma once
#include "LightCalculator.h"
#include "LightCommon.h"

#include <glm/glm.hpp>
#include <deque>
#include <map>
#include <set>
#include <vector>

#include <TRGame/Worlds/GameWorld.h>

struct Edge;
struct Vertex;

using PVertex = Vertex*;
using PEdge = Edge*;



struct EndPointInfo
{
	PEdge Edge;
	bool IsEnd;
};

struct Vertex
{
	glm::ivec2 Pos;
	std::vector<EndPointInfo> ConjunctionInfo{};

	Vertex(glm::ivec2 pos) : Pos(pos) {}

	glm::vec2 GetWorldPos() const
	{
		return glm::vec2(Pos * GameWorld::TILE_SIZE);
	}
};

struct Triangle
{
	glm::vec2 Pos[3];
	Triangle(glm::vec2 A, glm::vec2 B, glm::vec2 C) : Pos{ A, B, C }
	{

	}
};


struct Ray
{
	glm::vec2 Start, Dir;
	glm::vec2 Eval(float t) const
	{
		return Start + Dir * t;
	}
};

struct Edge
{
	glm::ivec2 Start, End;
	int Id;
	bool Horizontal;

	bool IsBorder() const { return Id < 4; }

	Edge(PVertex start, PVertex end, int id, bool horizontal) : Id(id), Horizontal(horizontal)
	{
		Start = start->Pos * GameWorld::TILE_SIZE;
		End = end->Pos * GameWorld::TILE_SIZE;
	}

	bool IntersectionTest(const Ray& ray, float& t) const
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
		if (t < -LightCommon::EPS) return false;

		double other = ray.Start[!Horizontal] + travel * ray.Dir[!Horizontal] / dv;
		double minn = std::min(Start[!Horizontal], End[!Horizontal]);
		double maxx = std::max(Start[!Horizontal], End[!Horizontal]);
		if (other < minn - LightCommon::EPS || other > maxx + LightCommon::EPS) return false;
		return true;
	}
};

struct KeyPointTmp
{
	PVertex Vertex;
	double Angle;

	bool operator<(const KeyPointTmp& p) const
	{
		return Angle < p.Angle;
	}
};

struct SegmentNodeMax
{
	PEdge Edge;
	float v;

	bool operator<(const SegmentNodeMax& node) const
	{
		return v < node.v;
	}
};

struct SegmentNodeMin
{
	PEdge Edge;
	float v;

	bool operator<(const SegmentNodeMin& node) const
	{
		return v > node.v;
	}
};

struct SweepStructure
{
	glm::vec2 lastKeyPosition{};
	std::set<PEdge> activeSegments{};
	std::set<PEdge> borderEdges{};
	std::priority_queue<SegmentNodeMax> MaxNode[2]{};
	std::priority_queue<SegmentNodeMin> MinNode[2]{};
};



class DirectionalLightCalculator : public LightCalculator
{
public:
	DirectionalLightCalculator(LightCommon* lightCommon);
	virtual ~DirectionalLightCalculator() override;

	virtual void AddLight(const Light& light) override;
	virtual void ClearLights() override;
	virtual void Calculate() override;

	void DrawTriangles(const glm::mat4& worldProjection);

private:
	LightCommon* _lightCommonData;

	std::vector<Light> _lights;

	std::deque<Vertex> _vertices;
	std::map<std::pair<int, int>, PVertex> _vertexPtrMap;
	std::deque<Edge> _edges;

	std::vector<Triangle> _shadowTriangles;

	PVertex getVertexPtr(glm::ivec2 pos);
	void calculateTrianglesForOneLight(const Light& light);

	void addBorderEdges(const trv2::RectI& rect, glm::vec2 sweepCenter);
	void addTileEdges(const trv2::RectI& rect, glm::vec2 sweepCenter);
	void addOneSegment(PVertex A, PVertex B, bool horizontal, glm::vec2 sweepCenter);

	void performFirstScan(const std::vector<KeyPointTmp>& sweep, SweepStructure& structure, 
		glm::vec2 sweepCenter, int& nxtIndex);

	void findNearestWall(const Ray& ray, std::deque<Edge>::iterator beginBorder,
		std::deque<Edge>::iterator endBorder,
		std::deque<Edge>::iterator endEdges,
		float& minnTime, PEdge& minnEdge);

	void findNearestWall(const Ray& ray, SweepStructure& structure,
		float& minnTime, PEdge& minnEdge);

	void performOneScan(const std::vector<KeyPointTmp>& sweep, SweepStructure& structure,
		glm::vec2 sweepCenter, int start, int end);

	void insertNewEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter);
	void eraseEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter);
};