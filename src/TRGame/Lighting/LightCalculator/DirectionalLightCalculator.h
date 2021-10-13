#pragma once
#include "LightCalculator.h"
#include "LightCommon.h"

#include <glm/glm.hpp>
#include <deque>
#include <map>
#include <set>
#include <vector>

#include <TRGame/Worlds/GameWorld.h>
#include <TREngine/Core/Utils/Utils.h>

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
	int Id;
	std::vector<EndPointInfo> ConjunctionInfo{};

	Vertex(int id, glm::ivec2 pos) : Id(id), Pos(pos) {}

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
	PVertex StartVertex;
	int Id;
	bool Horizontal;

	bool IsBorder() const { return Id < 4; }

	Edge(PVertex start, PVertex end, int id, bool horizontal) : Id(id), Horizontal(horizontal)
	{
		Start = start->Pos * GameWorld::TILE_SIZE;
		End = end->Pos * GameWorld::TILE_SIZE;
		StartVertex = start;
	}

	bool IntersectionTest(const Ray& ray, float& t, bool checkBorder = false) const
	{
		double dv = ray.Dir[Horizontal];
		double travel = Start[Horizontal] - ray.Start[Horizontal];
		if (dv == 0.0)
		{
			double a = Start[!Horizontal] / ray.Dir[!Horizontal];
			double b = End[!Horizontal] / ray.Dir[!Horizontal];
			if (a > b) std::swap(a, b);
			t = a;
			if (a < 0.0) t = b;
			return t >= 0.0 && travel == 0.0;
		}
		t = travel / dv;
		if (t < -LightCommon::EPS) return false;

		if (checkBorder)
		{
			double other = ray.Start[!Horizontal] + travel * ray.Dir[!Horizontal] / dv;
			double minn = std::min(Start[!Horizontal], End[!Horizontal]);
			double maxx = std::max(Start[!Horizontal], End[!Horizontal]);
			if (other < minn - LightCommon::EPS || other > maxx + LightCommon::EPS) return false;
		}
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


struct EdgeCmp;
//struct EdgeCmpNode;
using GeoPQ = std::set<PEdge, EdgeCmp>;
//using GeoPQ = MinPQ<PEdge, EdgeCmp>;

struct SweepStructure
{
	glm::vec2 lastKeyPosition{};
	bool activeEdges[1000];
	std::vector<PEdge> borderEdges{};
	int currentRound = 0;
	GeoPQ* EdgeSet;
	Ray currentRay{};
	Ray differentialRay{};

	SweepStructure(int maxEdges)
	{
		//activeEdges = std::make_unique<bool[]>(maxEdges);
		memset(activeEdges, 0, sizeof(bool) * maxEdges);
	}
};

//struct EdgeCmpNode
//{
//	std::vector<PEdge> Edges;
//	int Round = 0;
//};

struct EdgeCmp
{

	SweepStructure& structure;
	EdgeCmp(SweepStructure& structure) : structure(structure) {}

	bool cmp(PEdge A, PEdge B)  const
	{
		auto v = A->End - A->Start;
		auto v2 = B->End - A->Start;
		return cross2(v, v2) > 0;
	}
	bool operator() (PEdge A, PEdge B) const
	{
		bool a = cmp(A, B);
		if (a)
		{
			if (cmp(B, A)) return A->Id < B->Id;
			return true;
		}
		else
		{
			if(!cmp(B, A)) return A->Id < B->Id;
			return false;
		}
	}
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

	//void findNearestWall(const Ray& ray, std::deque<Edge>::iterator beginBorder,
	//	std::deque<Edge>::iterator endBorder,
	//	std::deque<Edge>::iterator endEdges,
	//	float& minnTime, PEdge& minnEdge);

	void findNearestWall(SweepStructure& structure,
		float& minnTime, PEdge& minnEdge);

	void performOneScan(const std::vector<KeyPointTmp>& sweep, SweepStructure& structure,
		glm::vec2 sweepCenter, int start, int end);

	void insertNewEdge(SweepStructure& structure, const std::vector<PEdge>& edges, glm::vec2 sweepCenter);
	void eraseEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter);
};