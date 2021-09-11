#include "GameWorld.hpp"
#include "Tile.hpp"


#include <random>
#include <TRGame/TRGame.hpp>
#include <TREngine/Engine.h>
#include <TREngine/Core/Assets/AssetsManager.h>
#include <TREngine/Core/Render/SpriteRenderer.h>

#include <TREngine/Core/Render/RenderTarget2D.h>
#include <TREngine/Graphics/Graphics_Interfaces.h>
#include <TREngine/Platform/Platform_Interfaces.h>

#include <glm/gtx/transform.hpp>
#include <algorithm>

static const glm::vec4 tempColorTable[5] = {
	glm::vec4(1),
	glm::vec4(0.6, 0.5, 0.3, 1.0),
	glm::vec4(0, 0, 1.0, 1.0),
	glm::vec4(0.3, 1.0, 0.1, 1.0),
	glm::vec4(0.4, 0.4, 0.4, 1.0),
};

std::mt19937 mt;

static constexpr int DIM = 50;
static constexpr float Magnifier = DIM;
static glm::vec2 randTable[55][55];


static float GetRandFloat() {
	return (float)mt() / mt.max();
}

static glm::vec2 SmoothInterp(glm::vec2 x)
{
	auto x2 = x * x;
	return (6.f * x2 - 15.f * x + 10.f) * x2 * x;
}

static float perlin(glm::vec2 coord)
{
	auto baseCoord = glm::ivec2(coord);
	baseCoord.x %= DIM;
	baseCoord.y %= DIM;
	auto extraCoord = glm::fract(coord);

	auto factor = SmoothInterp(extraCoord);

	float V1 = glm::dot(randTable[baseCoord.x][baseCoord.y], extraCoord - glm::vec2(0, 0));
	float V2 = glm::dot(randTable[baseCoord.x + 1][baseCoord.y], extraCoord - glm::vec2(1, 0));
	float V3 = glm::dot(randTable[baseCoord.x][baseCoord.y + 1], extraCoord - glm::vec2(0, 1));
	float V4 = glm::dot(randTable[baseCoord.x + 1][baseCoord.y + 1], extraCoord - glm::vec2(1, 1));

	float v1 = glm::mix(V1, V2, factor.x);
	float v2 = glm::mix(V3, V4, factor.x);
	return glm::mix(v1, v2, factor.y);
}

GameWorld::GameWorld(int width, int height) : _tileMaxX(width), _tileMaxY(height)
{
	_tiles = std::make_unique<Tile[]>(width * height);

	auto engine = TRGame::GetInstance()->GetEngine();
	auto graphicsDevice = engine->GetGraphicsDevice();
	auto resourceManager = engine->GetGraphicsResourceManager();

	_renderTarget = std::make_shared<trv2::RenderTarget2D>(resourceManager, 1024, 1024);

	for (int i = 0; i <= DIM; i++) {
		for (int j = 0; j <= DIM; j++) {
			float r = GetRandFloat() * glm::two_pi<float>();

			randTable[i][j] = glm::vec2(std::cos(r), std::sin(r));
		}
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			auto coord = glm::vec2((float)x / width, (float)y / height) * 10.f;

			auto v =0.f;
			
			float K = 1;
			for (int i = 0; i < 10; i++)
			{
				v += 1.f / K * perlin(coord * K);
				K *= 2;
			}

			//SetTile(x, y, Tile(glm::vec3(v) * 0.5f + 0.5f));
			v = sin(v * 7 + coord.y * 2);
			if (v < 0.5) {
				SetTile(x, y, Tile(glm::vec3(0.6, 0.5, 0.3)));
			}
			else {
				SetTile(x, y, Tile(glm::vec3(1, 1, 1)));
			}
			//else if(v > 0.3) {
			//	SetTile(x, y, Tile(2));
			//}
			//else {
			//	SetTile(x, y, Tile(3));
			//}
		}
	}
}

Tile& GameWorld::GetTile(int x, int y)
{
	return _tiles[y * _tileMaxX + x];
}

void GameWorld::SetTile(int x, int y, const Tile& tile)
{
	_tiles[y * _tileMaxX + x] = tile;
}


void GameWorld::RenderWorld(const glm::mat4& projection, trv2::SpriteRenderer* renderer, const trv2::Rect2D<float>& renderRect)
{
	// calculate draw rect
	glm::ivec2 botLeft((int)(renderRect.BottomLeft().x / GameWorld::TILE_SIZE), 
		(int)(renderRect.BottomLeft().y / GameWorld::TILE_SIZE));
	botLeft.x = std::max(0, std::min(_tileMaxX - 1, botLeft.x));
	botLeft.y = std::max(0, std::min(_tileMaxY - 1, botLeft.y));

	glm::ivec2 topRight((int)((renderRect.TopRight().x + GameWorld::TILE_SIZE - 1) / GameWorld::TILE_SIZE),
		(int)((renderRect.TopRight().y + GameWorld::TILE_SIZE - 1) / GameWorld::TILE_SIZE));
	topRight.x = std::max(0, std::min(_tileMaxX - 1, topRight.x));
	topRight.y = std::max(0, std::min(_tileMaxY - 1, topRight.y));

	trv2::Rect2D<int> viewRect(botLeft, topRight - botLeft);

	auto graphicsDevice = TRGame::GetInstance()->GetEngine()->GetGraphicsDevice();
	auto assetsManager = TRGame::GetInstance()->GetEngine()->GetAssetsManager();
	auto clientSize = TRGame::GetInstance()->GetEngine()->GetGameWindow()->GetWindowSize();

	//_spriteRenderer->Draw(glm::vec2((int)(worldPos.x / 16) * 16, (int)(worldPos.y / 16) * 16), glm::vec2(16), glm::vec2(0), 0.f, glm::vec4(1, 0, 0, 1));
	trv2::BatchSettings setting{};
	setting.SpriteSortMode = trv2::SpriteSortMode::Deferred;
	setting.Shader = assetsManager->GetShader("perlinNoise");

	// Render to offscreen texture
	graphicsDevice->SwitchRenderTarget(_renderTarget.get());
	graphicsDevice->SetViewPort(0, 0, _renderTarget->GetWidth(), _renderTarget->GetHeight());
	graphicsDevice->Clear(glm::vec4(0, 0, 0, 0));
	glm::mat4 localProj = glm::ortho<float>(0.f, _renderTarget->GetWidth(), 0.f, _renderTarget->GetHeight());
	renderer->Begin(localProj, setting);
	{
		renderer->Draw(glm::vec2(0), glm::vec2(1024, 1024), glm::vec2(0), 0.f, glm::vec4(1));
	}
	renderer->End();


	graphicsDevice->SwitchRenderTarget(nullptr);
	graphicsDevice->SetViewPort(0, 0, clientSize.x, clientSize.y);
	graphicsDevice->Clear(glm::vec4(0, 0, 0, 0));

	setting.Shader = nullptr;
	renderer->Begin(projection, setting);
	{
		renderer->Draw(_renderTarget->GetTexture2D(), glm::vec2(0), glm::vec2(1024, 1024), glm::vec2(0), 0.f, glm::vec4(1));
	}
	renderer->End();

	//auto start = glm::vec2(viewRect.Position);
	//auto assetManager = TRGame::GetInstance()->GetEngine()->GetAssetsManager();
	//for (int i = 0; i < viewRect.Size.x; i++)
	//{
	//	for (int j = 0; j < viewRect.Size.y; j++)
	//	{
	//		auto coord = viewRect.BottomLeft() + glm::ivec2(i, j);
	//		auto startPos = glm::vec2(coord) * (float)GameWorld::TILE_SIZE;
	//		auto& tile = GetTile(coord.x, coord.y);

	//		renderer->Draw(startPos, glm::vec2(TILE_SIZE), glm::vec2(0), 0.f, glm::vec4(tile.GetColor(), 1.f));
	//	}
	//}
}
