#include "TileSection.h"
#include "Tile.h"
#include "WorldMap.h"

#include <glm/gtx/transform.hpp>
#include <algorithm>

#include <random>
#include <TRGame/TRGame.hpp>
#include <TREngine/Engine.h>
#include <TREngine/Core/Assets/assets.h>
#include <TREngine/Core/Render/render.h>
#include <TREngine/Core/Utils/Logging/Logger.h>

#include <TREngine/Graphics/Graphics_Interfaces.h>
#include <TREngine/Platform/Platform_Interfaces.h>

#include <TRGame/Lighting/Lighting.h>

static const glm::vec3 tempColorTable[5] = {
	glm::vec3(0.f, 0.f, 0.f),
	glm::vec3(0.5, 0.5, 0.5),
	glm::vec3(1.0, 0.5, 0.2),
	glm::vec3(0.6, 0.6, 0.1),
	glm::vec3(0.4, 0.4, 0.4),
};


constexpr int MAXIMAL_SURFACE_HEIGHT = 950;
constexpr int MINIMAL_SURFACE_HEIGHT = 800;
constexpr int MINIMAL_SURFACE_THICKNESS = 50;


static constexpr int DIM = 50;
static constexpr float Magnifier = DIM;
static constexpr unsigned int MAX_UINT = 0xffffffff;
static constexpr int BASE1 = 998244353;
static constexpr int BASE2 = 993244853;
static constexpr int MOD = 1e9 + 7;
static constexpr int SEED = 1;
static constexpr int PNUM = 512;
static int Permutation[PNUM];

using uint32 = unsigned int;
using ull = unsigned long long int;

std::mt19937 mt(SEED);

static float GetRandFloat()
{
	constexpr unsigned s = mt.max();
	return (double)mt() / s;
}

static float GetRandValueGrid(int x, int y, int seed)
{
	return glm::fract(std::sin(glm::dot(glm::vec2(x, y), glm::vec2(12.9898, 78.233))) * 43758.5453
		+ std::cos(0.114514 * (x * y * seed)));
}




float GetRandValueGrid2(int x, int y, int seed)
{
	static constexpr int A = 0x7fffffff / 48271;
	static constexpr int B = 0x7fffffff % 48271;
	int result;

	seed = x ^ ((y << 1) & 0x2AAAAAAA) ^ ((y >> 1) & 0x33333333);
	result = 48271 * (seed % A) - B * (seed / A);
	return (float)(result & 0xffff) / 0xffff;
}

//static float GetRandValueGrid(uint32 x, uint32 y, uint32 seed)
//{
//	auto s = (ull)x * BASE % MOD + y;
//	auto s1 = s * BASE % MOD + seed;
//	auto s2 = s1 * BASE % MOD;
//	return (double)s2 / (MOD - 1);
//}

static glm::vec2 GetDir(int x, int y, int type)
{
	float r;
	if (type == 0)
	{
		r = GetRandValueGrid(x, y, SEED) * glm::two_pi<float>();
	}
	else
	{
		r = GetRandValueGrid2(x, y, SEED) * glm::two_pi<float>();
	}
	return glm::vec2(cos(r), sin(r));
}

static glm::vec2 SmoothInterp(glm::vec2 x)
{
	auto x2 = x * x;
	return (6.f * x2 - 15.f * x + 10.f) * x2 * x;
}

static float perlin(glm::vec2 coord, int type)
{
	auto P = glm::ivec2(glm::floor(coord));
	auto extraCoord = glm::fract(coord);

	auto factor = SmoothInterp(extraCoord);

	float V1 = glm::dot(GetDir(P.x, P.y, type), extraCoord - glm::vec2(0, 0));
	float V2 = glm::dot(GetDir(P.x + 1, P.y, type), extraCoord - glm::vec2(1, 0));
	float V3 = glm::dot(GetDir(P.x, P.y + 1, type), extraCoord - glm::vec2(0, 1));
	float V4 = glm::dot(GetDir(P.x + 1, P.y + 1, type), extraCoord - glm::vec2(1, 1));

	float v1 = glm::mix(V1, V2, factor.x);
	float v2 = glm::mix(V3, V4, factor.x);
	return glm::mix(v1, v2, factor.y);
}

static float fBm(glm::vec2 coord, int X, int type)
{
	float v = 0.f;
	float b = 0.f;
	float K = 1;
	for (int i = 0; i < X; i++)
	{
		v += 1.f / K * perlin(coord * K, type);
		b += 1.f / K;
		K *= 2;
	}
	return v;
}

TileSection::TileSection(glm::ivec2 start, glm::ivec2 size) : _sectionStart(start), _sectionSize(size)
{
	_tiles = std::make_unique<Tile[]>(size.x * size.y);
	_worldGenLayouts = std::make_unique<TileGenLayout[]>(size.x * size.y);

	_sectionMap = std::make_unique<WorldMap>(size);

	auto engine = TRGame::GetInstance()->GetEngine();
	auto logger = engine->GetLogger();
	auto resourceManager = engine->GetGraphicsResourceManager();


	for (int s = 0; s < 1; s++)
	{
		logger->Log(trv2::SeverityLevel::Info, "Generating Loop %d", s);

		float dx = 1.0 / size.x;
		float dy = 1.0 / size.y;
		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
				auto coord = glm::vec2(x * dx, y * dy) * 18.f;

				auto v = fBm(coord, 8, s);

				_worldGenLayouts[y * size.x + x].v[s] = v;
			}
		}
	}

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			glm::ivec2 coord(x, y);
			float v = _worldGenLayouts[y * size.x + x].v[0];
			float v2 = _worldGenLayouts[y * size.x + x].v[1];

			float threashold = 0.15f;
			auto& tile = GetTile(coord);

			if (y > MINIMAL_SURFACE_HEIGHT)
			{
				float f = (float)(y - MINIMAL_SURFACE_HEIGHT) / (MAXIMAL_SURFACE_HEIGHT - MINIMAL_SURFACE_HEIGHT);
				f = std::pow(f, 2.0);
				threashold = glm::mix(0.15f, 0.8f, f);
			}

			if (v < threashold)
			{
				tile.Type = 1;
				tile.Solid = true;
				if (v2 < -0.5)
				{
					tile.Type = 2;
					tile.Solid = true;
				}
			}
			else
			{
				tile.Type = 0;
			}
		}
	}


	for (int x = 0; x < size.x; x++)
	{

		float X = (x + 0.5f) / size.x;
		float v = fBm(glm::vec2(1.5, X * 5), 4, 0);
		v = sin(v) * 0.5 + 0.5;

		int BlockH = MAXIMAL_SURFACE_HEIGHT - MINIMAL_SURFACE_HEIGHT - MINIMAL_SURFACE_THICKNESS;
		float BaseLine = MINIMAL_SURFACE_HEIGHT + BlockH * v;

		float v1 = fBm(glm::vec2(2.5, X * 5), 4, 0);
		float v2 = fBm(glm::vec2(3.5, X * 5), 4, 0);
		float v3 = fBm(glm::vec2(4.5, X * 5), 4, 0);
		float bottom = BaseLine - MINIMAL_SURFACE_THICKNESS * v1;

		float AVHeight = MAXIMAL_SURFACE_HEIGHT - bottom - MINIMAL_SURFACE_THICKNESS;
		float top = bottom + MINIMAL_SURFACE_THICKNESS + AVHeight * v2;

		float botbot = bottom - MINIMAL_SURFACE_THICKNESS * 2 - MINIMAL_SURFACE_THICKNESS * 2 * v3;

		//for (int i = bottom; i <= top; i++)
		//{
		//	auto& tile = GetTile(x, i);

		//	tile.SetColor(glm::vec3(0.6, 0.5, 0.3));

		//}

		for (int i = botbot; i <= top; i++)
		{
			auto& tile = GetTile(glm::ivec2(x, i));

			if (!tile.IsEmpty())
			{
				tile.Type = 3;
				tile.Solid = true;
			}
		}


		for (int i = top; i < size.y; i++)
		{
			auto& tile = GetTile(glm::ivec2(x, i));
			tile.Type = 0;
		}
	}

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			const auto& tile = GetTile(glm::ivec2(x, y));
			_sectionMap->SetColor(glm::ivec2(x, y), tempColorTable[tile.Type]);
		}
	}
}

TileSection::~TileSection()
{

}


Tile& TileSection::GetTile(glm::ivec2 pos)
{
	pos.x = std::max(0, std::min(_sectionStart.x + _sectionSize.x - 1, pos.x));
	pos.y = std::max(0, std::min(_sectionStart.y + _sectionSize.y - 1, pos.y));
	return _tiles[pos.y * _sectionSize.x + pos.x];
}

const Tile& TileSection::GetTile(glm::ivec2 pos) const
{
	pos.x = std::max(0, std::min(_sectionStart.x + _sectionSize.x - 1, pos.x));
	pos.y = std::max(0, std::min(_sectionStart.y + _sectionSize.y - 1, pos.y));
	return _tiles[pos.y * _sectionSize.x + pos.x];
}

void TileSection::SetTile(glm::ivec2 pos, const Tile& tile)
{
	_tiles[pos.y * _sectionSize.x + pos.x] = tile;
}
