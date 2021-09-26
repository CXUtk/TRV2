#include "GameWorld.h"
#include "TileSection.h"
#include "WorldMap.h"
#include "Tile.h"

#include <glm/gtx/transform.hpp>
#include <algorithm>

#include <random>
#include <TRGame/TRGame.hpp>
#include <TREngine/Engine.h>
#include <TREngine/Core/Assets/assets.h>
#include <TREngine/Core/Render/render.h>
#include <TREngine/Core/Utils/Logging/Logger.h>
#include <TREngine/Core/Utils/Utils.h>

#include <TREngine/Graphics/Graphics_Interfaces.h>
#include <TREngine/Platform/Platform_Interfaces.h>

#include <TRGame/Lighting/Lighting.h>

GameWorld::GameWorld()
{
	
}

GameWorld::~GameWorld()
{
	
}

trv2::RectI GameWorld::GetTileRect(const trv2::RectI& worldRect)
{
	// calculate draw rect
	glm::ivec2 botLeft = GetLowerWorldCoord(worldRect.BottomLeft(), 32);
	glm::ivec2 topRight = GetUpperWorldCoord(worldRect.TopRight(), 32);
	return trv2::RectI(botLeft, topRight - botLeft);
}

trv2::RectI GameWorld::GetTileSectionRect(const trv2::RectI& tileRect)
{
	// calculate section rect
	glm::ivec2 botLeft = RoundDown(tileRect.BottomLeft(), TILE_SECTION_SIZE);
	glm::ivec2 topRight = RoundUp(tileRect.TopRight(), TILE_SECTION_SIZE);
	return trv2::RectI(botLeft, topRight - botLeft);
}

TileSection* GameWorld::getTileSection(glm::ivec2 sectionPos, const trv2::RectI& tileViewRect)
{
	int emptyIndex = -1;
	auto tileCoord = sectionPos * GameWorld::TILE_SECTION_SIZE;
	for (int i = 0; i < TILE_SECTION_CACHE_SIZE; i++)
	{
		if (_cachedSections[i] == nullptr)
		{
			emptyIndex = i;
		}
		else if (_cachedSections[i]->GetSectionStartPos() == tileCoord)
		{
			return _cachedSections[i].get();
		}
	}
	

	if(emptyIndex == -1)
	{
		for (int i = 0; i < TILE_SECTION_CACHE_SIZE; i++)
		{
			if (!_cachedSections[i]->Intersects(tileViewRect))
			{
				emptyIndex = i;
				break;
			}
		}
	}

	if (emptyIndex != -1)
	{
		_cachedSections[emptyIndex] = std::make_shared<TileSection>(tileCoord, glm::ivec2(TILE_SECTION_SIZE));
	}
	return _cachedSections[emptyIndex].get();
}

const TileSection* GameWorld::checkInCache(glm::ivec2 sectionPos) const
{
	auto tileCoord = sectionPos * GameWorld::TILE_SECTION_SIZE;
	for (int i = 0; i < TILE_SECTION_CACHE_SIZE; i++)
	{
		if (_cachedSections[i] != nullptr && _cachedSections[i]->GetSectionStartPos() == tileCoord)
		{
			return trv2::cptr(_cachedSections[i]);
		}
	}
	return nullptr;
}


void GameWorld::RenderWorld(const glm::mat4& projection, trv2::SpriteRenderer* renderer, const trv2::RectI& renderRect, trv2::RenderTarget2D* renderTarget)
{
	auto clientSize = trv2::Engine::GetInstance()->GetGameWindow()->GetWindowSize();

	auto tileRect = GetTileRect(renderRect);
	auto sectionRect = GetTileSectionRect(tileRect);

	for (int y = sectionRect.Position.y; y < sectionRect.Position.y + sectionRect.Size.y; y++)
	{
		for (int x = sectionRect.Position.x; x < sectionRect.Position.x + sectionRect.Size.x; x++)
		{
			glm::ivec2 tileCoord(x * GameWorld::TILE_SECTION_SIZE, y * GameWorld::TILE_SECTION_SIZE);
			glm::vec2 worldCoord = glm::vec2(tileCoord) * (float)GameWorld::TILE_SIZE;

			auto projSection = projection * glm::translate(glm::vec3(worldCoord, 0.f));
			TileSection* section = getTileSection(glm::ivec2(x, y), tileRect);
			assert(section != nullptr);
			section->RenderSection(projSection, renderer, renderTarget);
		}
	}
	
}

//Tile& GameWorld::GetTile(glm::ivec2 pos)
//{
//	static Tile empty;
//	empty.Type = 0;
//	return empty;
//}

const Tile& GameWorld::GetTile(glm::ivec2 pos) const
{
	static Tile empty;
	empty.Type = 0;

	auto sectionPos = RoundDown(pos, TILE_SECTION_SIZE);
	const TileSection* section = nullptr;
	if (!(section = checkInCache(sectionPos))) return empty;
	return section->GetTile(pos - section->GetSectionStartPos());
}

bool GameWorld::TileExists(glm::ivec2 pos) const
{
	auto sectionPos = RoundDown(pos, TILE_SECTION_SIZE);
	return checkInCache(sectionPos) != nullptr;
}

trv2::Texture2D* GameWorld::GetMapTexture()
{
	return nullptr;
}

glm::ivec2 GameWorld::GetLowerWorldCoord(glm::vec2 pos, int offscreenTiles)
{
	return RoundDown(glm::ivec2(pos - glm::vec2(offscreenTiles * TILE_SIZE)), TILE_SIZE);
}

glm::ivec2 GameWorld::GetUpperWorldCoord(glm::vec2 pos, int offscreenTiles)
{
	return RoundUp(glm::ivec2(pos + glm::vec2(offscreenTiles * TILE_SIZE)), TILE_SIZE);
}