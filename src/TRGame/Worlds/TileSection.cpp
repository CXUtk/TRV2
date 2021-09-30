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
#include <TRGame/Worlds/GameWorld.h>

static trv2::Texture2D* tileTextureTable[5];


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

	float V1 = glm::dot(GetDir(P.x, P.y, type), extraCoord - glm::vec2(0, 0));
	float V2 = glm::dot(GetDir(P.x + 1, P.y, type), extraCoord - glm::vec2(1, 0));
	float V3 = glm::dot(GetDir(P.x, P.y + 1, type), extraCoord - glm::vec2(0, 1));
	float V4 = glm::dot(GetDir(P.x + 1, P.y + 1, type), extraCoord - glm::vec2(1, 1));

	auto factor = SmoothInterp(extraCoord);

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

TileSection::TileSection(glm::ivec2 tileStart, glm::ivec2 tileSize) : _sectionStart(tileStart), _sectionSize(tileSize)
{
	_tiles = std::make_unique<Tile[]>(_sectionSize.x * _sectionSize.y);
	_worldGenLayouts = std::make_unique<TileGenLayout[]>(_sectionSize.x * _sectionSize.y);

	_sectionMap = std::make_unique<WorldMap>(_sectionSize);

	trv2::TextureParameters texPara{};
	texPara.InternalFormat = trv2::PixelFormat::RGBA;
	_cacheRenderTargetWalls = std::make_unique<trv2::RenderTarget2D>(trv2::Engine::GetInstance()->GetGraphicsResourceManager(),
		tileSize * GameWorld::TILE_SIZE, texPara);
	_cacheRenderTargetTiles = std::make_unique<trv2::RenderTarget2D>(trv2::Engine::GetInstance()->GetGraphicsResourceManager(),
		tileSize * GameWorld::TILE_SIZE, texPara);

	auto engine = TRGame::GetInstance()->GetEngine();
	auto logger = engine->GetLogger();
	auto resourceManager = engine->GetGraphicsResourceManager();


	for (int s = 0; s < 1; s++)
	{
		logger->Log(trv2::SeverityLevel::Info, "Generating Tile Section at (%d, %d)", tileStart.x, tileStart.y);

		float dx = 1.0 / _sectionSize.x;
		float dy = 1.0 / _sectionSize.y;
		for (int y = 0; y < _sectionSize.y; y++)
		{
			for (int x = 0; x < _sectionSize.x; x++)
			{
				auto coord = glm::vec2((x + _sectionStart.x) * dx, (y + _sectionStart.y) * dy) * 1.f;

				auto v = fBm(coord, 4, s);

				_worldGenLayouts[y * _sectionSize.x + x].v[s] = v;
			}
		}
	}

	for (int y = 0; y < _sectionSize.y; y++)
	{
		for (int x = 0; x < _sectionSize.x; x++)
		{
			glm::ivec2 coord(x, y);
			float v = _worldGenLayouts[y * _sectionSize.x + x].v[0];
			float v2 = _worldGenLayouts[y * _sectionSize.x + x].v[1];

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
				tile.Wall = 1;
			}
		}
	}


	//for (int x = 0; x < _sectionSize.x; x++)
	//{
	//	auto worldTileX = _sectionStart.x + x;

	//	float X = (worldTileX + 0.5f) / _sectionSize.x;
	//	float v = fBm(glm::vec2(1.5, X), 2, 0);
	//	v = sin(v) * 0.5 + 0.5;

	//	int BlockH = MAXIMAL_SURFACE_HEIGHT - MINIMAL_SURFACE_HEIGHT - MINIMAL_SURFACE_THICKNESS;
	//	float BaseLine = MINIMAL_SURFACE_HEIGHT + BlockH * v;

	//	float v1 = fBm(glm::vec2(2.5, X), 2, 0);
	//	float v2 = fBm(glm::vec2(3.5, X), 2, 0);
	//	float v3 = fBm(glm::vec2(4.5, X), 2, 0);
	//	float bottom = BaseLine - MINIMAL_SURFACE_THICKNESS * v1;

	//	float AVHeight = MAXIMAL_SURFACE_HEIGHT - bottom - MINIMAL_SURFACE_THICKNESS;
	//	float top = bottom + MINIMAL_SURFACE_THICKNESS + AVHeight * v2;

	//	float botbot = bottom - MINIMAL_SURFACE_THICKNESS * 2 - MINIMAL_SURFACE_THICKNESS * 2 * v3;

	//	//for (int i = bottom; i <= top; i++)
	//	//{
	//	//	auto& tile = GetTile(x, i);

	//	//	tile.SetColor(glm::vec3(0.6, 0.5, 0.3));

	//	//}

	//	for (int i = botbot; i <= top; i++)
	//	{
	//		auto& tile = GetTile(glm::ivec2(x, i));

	//		if (!tile.IsEmpty())
	//		{
	//			tile.Type = 3;
	//			tile.Solid = true;
	//		}
	//	}


	//	for (int i = top; i < size.y; i++)
	//	{
	//		auto& tile = GetTile(glm::ivec2(x, i));
	//		tile.Type = 0;
	//	}
	//}

	for (int y = 0; y < _sectionSize.y; y++)
	{
		for (int x = 0; x < _sectionSize.x; x++)
		{
			const auto& tile = GetTile(glm::ivec2(x, y));
			_sectionMap->SetColor(glm::ivec2(x, y), tile.IsAir() ? glm::vec3(0): glm::vec3(0.5));
		}
	}
}

TileSection::~TileSection()
{

}


Tile& TileSection::GetTile(glm::ivec2 pos)
{
	assert(pos.x >= 0 && pos.x < _sectionSize.x);
	assert(pos.y >= 0 && pos.y < _sectionSize.y);
	return _tiles[pos.y * _sectionSize.x + pos.x];
}

const Tile& TileSection::GetTile(glm::ivec2 pos) const
{
	assert(pos.x >= 0 && pos.x < _sectionSize.x);
	assert(pos.y >= 0 && pos.y < _sectionSize.y);
	return _tiles[pos.y * _sectionSize.x + pos.x];
}

void TileSection::SetTile(glm::ivec2 pos, const Tile& tile)
{
	assert(pos.x >= 0 && pos.x < _sectionSize.x);
	assert(pos.y >= 0 && pos.y < _sectionSize.y);
	_tiles[pos.y * _sectionSize.x + pos.x] = tile;
}

bool TileSection::Intersects(const trv2::RectI& tileRect) const
{
	return RectIntersects(trv2::RectI(_sectionStart, _sectionSize), tileRect);
}

void TileSection::RenderSection(const glm::mat4& projection, trv2::SpriteRenderer* renderer, trv2::RenderTarget2D* renderTarget)
{
	if (_isDirty)
	{
		reDrawCache(renderTarget);
	}

	//_spriteRenderer->Draw(glm::vec2((int)(worldPos.x / 16) * 16, (int)(worldPos.y / 16) * 16), glm::vec2(16), glm::vec2(0), 0.f, glm::vec4(1, 0, 0, 1));
	trv2::BatchSettings setting{};
	setting.SpriteSortMode = trv2::SpriteSortMode::Deferred;
	setting.BlendMode = trv2::BlendingMode::AlphaBlend;
	setting.Shader = nullptr;
	renderer->Begin(projection, setting);
	{
		renderer->Draw(_cacheRenderTargetWalls->GetTexture2D(), glm::vec2(0), _sectionSize * GameWorld::TILE_SIZE, glm::vec2(0), 0.f, glm::vec4(1));
		renderer->Draw(_cacheRenderTargetTiles->GetTexture2D(), glm::vec2(0), _sectionSize * GameWorld::TILE_SIZE, glm::vec2(0), 0.f, glm::vec4(1));

		// DEBUG
		//renderer->Draw(glm::vec2(0), glm::vec2(_sectionSize.x * 16, 1), glm::vec2(0),
		//			0.f, glm::vec4(0, 1, 0, 1));
		//renderer->Draw(glm::vec2(0), glm::vec2(1, _sectionSize.y * 16), glm::vec2(0),
		//	0.f, glm::vec4(0, 1, 0, 1));
		//renderer->Draw(glm::vec2(0, _sectionSize.y * 16 - 1), glm::vec2(_sectionSize.x * 16, 1), glm::vec2(0),
		//	0.f, glm::vec4(0, 1, 0, 1));
		//renderer->Draw(glm::vec2(_sectionSize.x * 16 - 1, 0), glm::vec2(1, _sectionSize.y * 16), glm::vec2(0),
		//	0.f, glm::vec4(0, 1, 0, 1));
	}
	renderer->End();
}

void TileSection::reDrawCache(trv2::RenderTarget2D* renderTarget)
{
	auto graphicsDevice = trv2::Engine::GetInstance()->GetGraphicsDevice();
	auto renderer = trv2::Engine::GetInstance()->GetSpriteRenderer();
	auto assetManager = trv2::Engine::GetInstance()->GetAssetsManager();

	trv2::BatchSettings setting{};
	setting.SpriteSortMode = trv2::SpriteSortMode::Deferred;
	setting.Shader = nullptr;
	auto canvasSize = _sectionSize * GameWorld::TILE_SIZE;
	auto projection = glm::ortho(0.f, (float)canvasSize.x, 0.f, (float)canvasSize.y);

	graphicsDevice->SwitchRenderTarget(trv2::ptr(_cacheRenderTargetWalls));
	graphicsDevice->Clear(glm::vec4(0));
	setting.Shader = assetManager->GetShader("tex:brick");
	renderer->Begin(projection, setting);
	{
		graphicsDevice->UseShader(setting.Shader);
		for (int y = 0; y < _sectionSize.y; y++)
		{
			for (int x = 0; x < _sectionSize.x; x++)
			{
				auto coord = glm::ivec2(x, y);
				auto startPos = glm::vec2(coord) * (float)GameWorld::TILE_SIZE;
				auto& tile = GetTile(coord);
				if (tile.Wall == 0)
				{
					continue;
				}
				renderer->Draw(startPos,
					glm::vec2(GameWorld::TILE_SIZE),  trv2::Rectf(_sectionStart + coord, glm::vec2(1)), glm::vec2(0),
					0.f, glm::vec4(1));
			}
		}
	}
	renderer->End();


	graphicsDevice->SwitchRenderTarget(trv2::ptr(_cacheRenderTargetTiles));
	graphicsDevice->Clear(glm::vec4(0));
	setting.Shader = assetManager->GetShader("tex:rock");
	renderer->Begin(projection, setting);
	{
		for (int y = 0; y < _sectionSize.y; y++)
		{
			for (int x = 0; x < _sectionSize.x; x++)
			{
				auto coord = glm::ivec2(x, y);
				auto startPos = glm::vec2(coord) * (float)GameWorld::TILE_SIZE;
				auto& tile = GetTile(coord);
				if (tile.Type == 0)
				{
					continue;
				}
				renderer->Draw(startPos,
					glm::vec2(GameWorld::TILE_SIZE), trv2::Rectf(_sectionStart + coord, glm::vec2(1)), glm::vec2(0),
					0.f, glm::vec4(1));
			}
		}
	}
	renderer->End();
	graphicsDevice->SwitchRenderTarget(renderTarget);

	_isDirty = false;
}
