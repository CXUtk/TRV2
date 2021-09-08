#include "GameWorld.hpp"
#include "Tile.hpp"

#include <TREngine/Utils/Structures/Rect.hpp>
#include <TREngine/Graphics/Renderers/SpriteRenderer.hpp>
#include <random>
#include <TRGame/TRGame.hpp>
#include <TREngine/TREngine.hpp>
#include <TREngine/Assets/AssetsManager.hpp>

static const glm::vec4 tempColorTable[5] = {
	glm::vec4(1),
	glm::vec4(0.6, 0.5, 0.3, 1.0),
	glm::vec4(0, 0, 1.0, 1.0),
	glm::vec4(0.3, 1.0, 0.1, 1.0),
	glm::vec4(0.4, 0.4, 0.4, 1.0),
};

std::mt19937 mt;
static constexpr float Magnifier = 50.f;

static float GetRandFloat() {
	return (float)mt() / mt.max();
}

GameWorld::GameWorld(int width, int height) : _width(width), _height(height)
{
	_tiles = std::make_unique<Tile[]>(width * height);
	float randTable[101][101];
	for (int i = 0; i <= 100; i++) {
		for (int j = 0; j <= 100; j++) {
			randTable[i][j] = GetRandFloat();
		}
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			auto coord = glm::vec2((float)x / width, (float)y / height) * Magnifier;

			auto baseCoord = glm::ivec2(coord);
			auto extraCoord = glm::fract(coord);

			extraCoord = glm::smoothstep(glm::vec2(0), glm::vec2(1), extraCoord);

			float v1 = glm::mix(randTable[baseCoord.x][baseCoord.y], randTable[baseCoord.x + 1][baseCoord.y], extraCoord.x);
			float v2 = glm::mix(randTable[baseCoord.x][baseCoord.y + 1], randTable[baseCoord.x + 1][baseCoord.y + 1], extraCoord.x);
			float v = glm::mix(v1, v2, extraCoord.y);

			if (v > 0.7) {
				SetTile(x, y, Tile(0));
			}
			else if(v > 0.5) {
				SetTile(x, y, Tile(1));
			}
			else if(v > 0.3) {
				SetTile(x, y, Tile(2));
			}
			else {
				SetTile(x, y, Tile(3));
			}
		}
	}
}

Tile& GameWorld::GetTile(int x, int y)
{
	return _tiles[y * _width + x];
}

void GameWorld::SetTile(int x, int y, const Tile& tile)
{
	_tiles[y * _width + x] = tile;
}


void GameWorld::RenderWorld(trv2::SpriteRenderer* renderer, const trv2::RectI& renderRect)
{
	auto start = glm::vec2(renderRect.Position);
	auto assetManager = TRGame::GetInstance()->GetEngine()->GetAssetsManager();
	for (int i = 0; i < renderRect.Size.x; i++) {
		for (int j = 0; j < renderRect.Size.y; j++) {
			auto coord = renderRect.Position + glm::ivec2(i, j);
			auto startPos = glm::vec2(coord) * (float)GameWorld::TILE_SIZE;
			auto& tile = GetTile(coord.x, coord.y);

			renderer->Draw(startPos, glm::vec2(TILE_SIZE), glm::vec2(0), 0.f, tempColorTable[tile.GetType()]);
		}
	}
}
