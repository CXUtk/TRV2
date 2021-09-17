#include "GameWorld.hpp"
#include "Tile.hpp"


#include <random>
#include <TRGame/TRGame.hpp>
#include <TREngine/Engine.h>
#include <TREngine/Core/Assets/AssetsManager.h>
#include <TREngine/Core/Render/SpriteRenderer.h>
#include <TREngine/Core/Utils/Logging/Logger.h>

#include <TREngine/Core/Render/RenderTarget2D.h>
#include <TREngine/Graphics/Graphics_Interfaces.h>
#include <TREngine/Platform/Platform_Interfaces.h>

#include <TRGame/Lighting/Lighting.h>

#include <glm/gtx/transform.hpp>
#include <algorithm>

static const glm::vec4 tempColorTable[5] = {
	glm::vec4(1),
	glm::vec4(0.6, 0.5, 0.3, 1.0),
	glm::vec4(0, 0, 1.0, 1.0),
	glm::vec4(0.3, 1.0, 0.1, 1.0),
	glm::vec4(0.4, 0.4, 0.4, 1.0),
};

static constexpr int DIM = 50;
static constexpr float Magnifier = DIM;
static constexpr unsigned int MAX_UINT = 0xffffffff;
static constexpr int BASE = 341243;
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

static float GetRandValueGrid(unsigned int x, unsigned int y, int seed)
{
	return glm::fract(std::sin(glm::dot(glm::vec2(x, y), glm::vec2(12.9898, 78.233))) * 43758.5453
		+ 114.514 * (x + y));
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


constexpr int MAXIMAL_SURFACE_HEIGHT = 950;
constexpr int MINIMAL_SURFACE_HEIGHT = 800;
constexpr int MINIMAL_SURFACE_THICKNESS = 50;

GameWorld::GameWorld(int width, int height) : _tileMaxX(width), _tileMaxY(height)
{
	_tiles = std::make_unique<Tile[]>(width * height);
	_worldGenLayouts = std::make_unique<TileGenLayout[]>(width * height);

	auto engine = TRGame::GetInstance()->GetEngine();
	auto logger = engine->GetLogger();
	auto graphicsDevice = engine->GetGraphicsDevice();
	auto resourceManager = engine->GetGraphicsResourceManager();


	for (int s = 0; s < 1; s++)
	{
		logger->Log(trv2::SeverityLevel::Info, "Generating Loop %d", s);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto coord = glm::vec2((float)x / width, (float)y / height) * 18.f;

				auto v = fBm(coord, 8, s);

				auto& tile = GetTile(x, y);

				_worldGenLayouts[y * width + x].v[s] = v;
			}
		}
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float v = _worldGenLayouts[y * width + x].v[0];
			float v2 = _worldGenLayouts[y * width + x].v[1];

			//v = std::pow(v * 1.2, 1.8);

			float threashold = 0.15f;
			auto& tile = GetTile(x, y);
			if (y > MINIMAL_SURFACE_HEIGHT)
			{
				float f = (float)(y - MINIMAL_SURFACE_HEIGHT) / (MAXIMAL_SURFACE_HEIGHT - MINIMAL_SURFACE_HEIGHT);
				f = std::pow(f, 2.0);
				threashold = glm::mix(0.15f, 0.8f, f);
			}
			//tile.SetColor(glm::vec3(v * 0.5 + 0.5));
			if (v < threashold)
			{
				tile.SetColor(glm::vec3(0.5, 0.5, 0.5));
				tile.SetSolid(true);
				if (v2 < -0.5)
				{
					tile.SetColor(glm::vec3(1, 0.5, 0.3));
					tile.SetSolid(true);
				}
			}
			else
			{
				tile.SetEmpty(true);
			}
		}
	}


	for (int x = 0; x < _tileMaxX; x++)
	{

		float X = (x + 0.5f) / _tileMaxX;
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
			auto& tile = GetTile(x, i);

			if (tile.GetColor() != glm::vec3(1))
			{
				tile.SetColor(glm::vec3(0.6, 0.5, 0.3));
				tile.SetSolid(true);
			}
		}


		for (int i = top; i < _tileMaxY; i++)
		{
			auto& tile = GetTile(x, i);
			tile.SetEmpty(true);
		}
	}


}

Tile& GameWorld::GetTile(int x, int y)
{
	x = std::max(0, std::min(_tileMaxX - 1, x));
	y = std::max(0, std::min(_tileMaxY - 1, y));
	return _tiles[y * _tileMaxX + x];
}

const Tile& GameWorld::GetTile(int x, int y) const
{
	x = std::max(0, std::min(_tileMaxX - 1, x));
	y = std::max(0, std::min(_tileMaxY - 1, y));
	return _tiles[y * _tileMaxX + x];
}

void GameWorld::SetTile(int x, int y, const Tile& tile)
{
	_tiles[y * _tileMaxX + x] = tile;
}


trv2::RectI GameWorld::GetTileRect(const trv2::Rectf& worldRect) const
{
	// calculate draw rect
	glm::ivec2 botLeft = GameWorld::GetLowerWorldCoord(worldRect.BottomLeft());
	botLeft.x = std::max(0, std::min(_tileMaxX - 1, botLeft.x));
	botLeft.y = std::max(0, std::min(_tileMaxY - 1, botLeft.y));



	glm::ivec2 topRight = GameWorld::GetUpperWorldCoord(worldRect.TopRight());
	topRight.x = std::max(0, std::min(_tileMaxX - 1, topRight.x));
	topRight.y = std::max(0, std::min(_tileMaxY - 1, topRight.y));

	return trv2::Rect2D<int>(botLeft, topRight - botLeft);
}

void GameWorld::RenderWorld(const glm::mat4& projection, trv2::SpriteRenderer* renderer, const trv2::Rect2D<float>& renderRect)
{
	auto graphicsDevice = TRGame::GetInstance()->GetEngine()->GetGraphicsDevice();
	auto assetsManager = TRGame::GetInstance()->GetEngine()->GetAssetsManager();
	auto clientSize = TRGame::GetInstance()->GetEngine()->GetGameWindow()->GetWindowSize();

	auto tileRect = GetTileRect(renderRect);

	//_spriteRenderer->Draw(glm::vec2((int)(worldPos.x / 16) * 16, (int)(worldPos.y / 16) * 16), glm::vec2(16), glm::vec2(0), 0.f, glm::vec4(1, 0, 0, 1));
	trv2::BatchSettings setting{};
	setting.SpriteSortMode = trv2::SpriteSortMode::Deferred;

	setting.Shader = nullptr;
	renderer->Begin(projection, setting);
	{
		auto start = glm::vec2(tileRect.Position);
		auto assetManager = TRGame::GetInstance()->GetEngine()->GetAssetsManager();
		for (int i = 0; i < tileRect.Size.x; i++)
		{
			for (int j = 0; j < tileRect.Size.y; j++)
			{
				auto coord = tileRect.BottomLeft() + glm::ivec2(i, j);
				auto startPos = glm::vec2(coord) * (float)GameWorld::TILE_SIZE;
				auto& tile = GetTile(coord.x, coord.y);
				if (tile.IsEmpty()) continue;

				renderer->Draw(startPos, glm::vec2(TILE_SIZE), glm::vec2(0), 0.f, glm::vec4(tile.GetColor(), 1.f));
			}
		}
	}
	renderer->End();
}

glm::ivec2 GameWorld::GetLowerWorldCoord(glm::vec2 pos)
{
	return glm::ivec2((pos.x - TILE_SIZE + 1) / TILE_SIZE, (pos.y - TILE_SIZE + 1) / TILE_SIZE);
}

glm::ivec2 GameWorld::GetUpperWorldCoord(glm::vec2 pos)
{
	return glm::ivec2((pos.x + TILE_SIZE) / TILE_SIZE, (pos.y + TILE_SIZE) / TILE_SIZE);
}
