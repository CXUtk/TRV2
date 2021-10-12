#pragma once
#include "LightCalculator.h"
#include "LightCommon.h"

#include <glm/glm.hpp>
#include <deque>
#include <map>
#include <set>
#include <vector>

#include <TRGame/Worlds/GameWorld.h>

template <typename T, typename PR>
struct MinPQ
{
	static const int MAX_NUM = 5005;
	T heap[MAX_NUM];
	PR _pr;
	int n;

	MinPQ() { n = 0; }
	~MinPQ() {}

	MinPQ(PR pr) :_pr(pr)
	{
		n = 0;
	}

	inline void push(T item)
	{
		heap[++n] = item;
		swim(n);
	}
	inline T top() const { return heap[1]; }
	inline void pop()
	{
		if (empty()) return;
		std::swap(heap[1], heap[n--]);
		sink(1);
	}
	inline void clear() { n = 0; }
	inline bool empty() const { return n == 0; }

	inline void swim(int k)
	{
		while (k > 1 && _pr(heap[k >> 1], heap[k]))
		{
			std::swap(heap[k >> 1], heap[k]);
			k >>= 1;
		}
	}

	inline void sink(int k)
	{
		while ((k << 1) <= n)
		{
			int j = k << 1;
			if (j < n && _pr(heap[j], heap[j + 1])) j++;
			if (!(heap[k] > heap[j])) break;
			std::swap(heap[k], heap[j]);
			k = j;
		}
	}
};


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
struct EdgeCmpNode;
using GeoPQ = std::priority_queue<PEdge, std::vector<PEdge>, EdgeCmp>;
//using GeoPQ = MinPQ<PEdge, EdgeCmp>;

struct SweepStructure
{
	glm::vec2 lastKeyPosition{};
	std::unique_ptr<bool[]> activeEdges;
	std::vector<PEdge> borderEdges{};
	GeoPQ* PQ = nullptr;
	Ray currentRay{};
	Ray differentialRay{};

	SweepStructure(int maxEdges)
	{
		activeEdges = std::make_unique<bool[]>(maxEdges);
		memset(activeEdges.get(), 0, sizeof(bool) * maxEdges);
		for (int i = 0; i < maxEdges; i++)
		{
			if (activeEdges[i])
			{
				assert(false);
			}
		}
	}
};

struct EdgeCmpNode
{
	std::vector<PEdge> Edges;
};

struct EdgeCmp
{

	SweepStructure& structure;
	EdgeCmp(SweepStructure& structure) : structure(structure) {}

	bool operator() (PEdge A, PEdge B) const
	{
		bool hasA = structure.activeEdges[A->Id];
		bool hasB = structure.activeEdges[B->Id];
		if (hasA != hasB) return hasA < hasB;
		if (!hasA) return A->Id < B->Id;
		float t1, t2;
		A->IntersectionTest(structure.differentialRay, t1);
		B->IntersectionTest(structure.differentialRay, t2);
		return t1 > t2;
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

	void insertNewEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter);
	void eraseEdge(SweepStructure& structure, PEdge edge, glm::vec2 sweepCenter);
};