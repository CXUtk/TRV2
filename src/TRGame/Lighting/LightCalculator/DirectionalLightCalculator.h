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
		//glm::vec2 b = glm::vec2((Pos.x * Pos.y + 1) % 7 - 3, (Pos.x * Pos.y + 3) % 7 - 3);
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
	glm::vec2 Start, End;
	PVertex StartVertex;
	int Id;
	bool Horizontal;

	bool IsBorder() const { return Id < 4; }

	Edge(PVertex start, PVertex end, int id, bool horizontal) : Id(id), Horizontal(horizontal)
	{
		Start = start->GetWorldPos();
		End = end->GetWorldPos();
		StartVertex = start;
	}

	bool IntersectionTest(const Ray& ray, float& t, bool checkBorder = false) const
	{
		glm::vec2 CA = ray.Start - Start;
		glm::mat2 M(End - Start, -ray.Dir);
		float a = std::abs(glm::determinant(M));
		//if (a < LightCommon::EPS)
		//{
		//	float s = std::abs(cross2(ray.Start - Start, End - Start));
		//	if (s < LightCommon::EPS)
		//	{
		//		t = ((Start - ray.Start) / ray.Dir).x;
		//		return true;
		//	}
		//	else
		//	{
		//		return false;
		//	}
		//}
		auto res = glm::inverse(M) * CA;
		t = res.y;
		float t2 = res.x;

		if (checkBorder)
		{
			if (t < -LightCommon::EPS) return false;
			if (t2 < -LightCommon::EPS || t2 > 1.f + LightCommon::EPS) return false;
		}
		float dist = glm::length(ray.Dir) * t;
		return !glm::isinf(t) && !glm::isnan(t);
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
	std::vector<PEdge> borderEdges{};
	GeoPQ* EdgeSet;
	Ray currentRay{};

	SweepStructure(int maxEdges)
	{
		//activeEdges = std::make_unique<bool[]>(maxEdges);
		//memset(activeEdges, 0, sizeof(bool) * maxEdges);
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

	//bool cmp(PEdge A, PEdge B)  const
	//{
	//	auto v = A->End - A->Start;
	//	auto v1 = B->Start - A->Start;
	//	auto v2 = B->End - A->Start;
	//	bool c1 = cross2(v, v1) > 0;
	//	bool c2 = cross2(v, v2) > 0;
	//	return (c1 && c2);
	//}

	bool inRange(glm::vec2 A, glm::vec2 B, glm::vec2 P) const
	{
		auto C = structure.currentRay.Start;
		auto v1 = A - C;
		auto v2 = C - B;
		return cross2(v1, P - C) >= 0 && cross2(v2, P - C) >= 0;
	}

	bool toRight(glm::vec2 A, glm::vec2 B, glm::vec2 P) const
	{
		return cross2(B - A, P - A) < 0;
	}

	bool cmp(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D) const
	{
		bool a = toRight(A, B, C);
		bool b = toRight(A, B, D);
		return a || b;
	}

	bool operator() (PEdge A, PEdge B) const
	{
		bool s = inRange(A->Start, A->End, B->Start);
		bool t = inRange(A->Start, A->End, B->End);
		if (s && t)
		{
			return cmp(A->Start, A->End, B->Start, B->End);
		}
		else
		{
			glm::vec2 start, end;
			float time;
			auto C = structure.currentRay.Start;
			auto R1 = Ray{ C, A->Start - C };
			if (!s && B->IntersectionTest(R1, time, true))
			{
				start = R1.Eval(time);
			}
			else
			{
				start = B->Start;
			}
			auto R2 = Ray{ C, A->End - C };
			if (!t && B->IntersectionTest(R2, time, true))
			{
				end = R2.Eval(time);
			}
			else
			{
				end = B->End;
			}
			return cmp(A->Start, A->End, start, end);
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
	void RasterizeLightTriangles();

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

	void insertNewEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter);
	void eraseEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter);
};