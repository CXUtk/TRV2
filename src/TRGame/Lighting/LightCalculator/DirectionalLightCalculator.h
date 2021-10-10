#pragma once
#include "LightCalculator.h"
#include "LightCommon.h"

#include <glm/glm.hpp>
#include <deque>
#include <map>
#include <vector>

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
	bool Horizontal;

	Edge(PVertex start, PVertex end, bool horizontal) : Horizontal(horizontal)
	{
		Start = start->Pos * 16;
		End = end->Pos * 16;
	}

	bool IntersectionTest(const Ray& ray, float& t)
	{
		double dv = ray.Dir[Horizontal];
		double travel = Start[Horizontal] - ray.Start[Horizontal];
		if (dv == 0.0) return false;
		t = travel / dv;
		if (t < -LightCommon::EPS) return false;

		double other = ray.Start[!Horizontal] + travel * ray.Dir[!Horizontal] / dv;
		double minn = std::min(Start[!Horizontal], End[!Horizontal]);
		double maxx = std::max(Start[!Horizontal], End[!Horizontal]);
		if (other < minn - LightCommon::EPS || other > maxx + LightCommon::EPS) return false;
		return true;
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
};