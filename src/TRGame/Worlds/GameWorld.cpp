#include "GameWorld.h"
#include "Tile.hpp"
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



static const glm::vec4 tempColorTable[5] = {
	glm::vec4(1),
	glm::vec4(0.6, 0.5, 0.3, 1.0),
	glm::vec4(0, 0, 1.0, 1.0),
	glm::vec4(0.3, 1.0, 0.1, 1.0),
	glm::vec4(0.4, 0.4, 0.4, 1.0),
};





GameWorld::GameWorld(int width, int height) : _tileMaxX(width), _tileMaxY(height)
{

	_worldMap = std::make_unique<WorldMap>(glm::ivec2(width, height));

	

}

GameWorld::~GameWorld()
{}

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

trv2::Texture2D* GameWorld::GetMapTexture()
{
	return _worldMap->GetTexture();
}

glm::ivec2 GameWorld::GetLowerWorldCoord(glm::vec2 pos)
{
	return glm::ivec2((pos.x - TILE_SIZE + 1) / TILE_SIZE, (pos.y - TILE_SIZE + 1) / TILE_SIZE);
}

glm::ivec2 GameWorld::GetUpperWorldCoord(glm::vec2 pos)
{
	return glm::ivec2((pos.x + TILE_SIZE) / TILE_SIZE, (pos.y + TILE_SIZE) / TILE_SIZE);
}
