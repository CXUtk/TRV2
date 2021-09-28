#pragma once
#include <memory>

#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>


class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void RenderWorld(const glm::mat4& projection, trv2::SpriteRenderer* renderer, const trv2::RectI& renderRect, trv2::RenderTarget2D* renderTarget);
	// Tile& GetTile(glm::ivec2 pos);
	const Tile& GetTile(glm::ivec2 pos) const;
	bool TileExists(glm::ivec2 pos) const;

	void FlushSectionCache(glm::ivec2 sectionPos);

	void RenderMapTexture(const glm::mat4& worldProjection, trv2::SpriteRenderer* renderer, glm::ivec2 screenSize);

	static glm::ivec2 GetLowerWorldCoord(glm::vec2 pos, int offscreenTiles = 1);
	static glm::ivec2 GetUpperWorldCoord(glm::vec2 pos, int offscreenTiles = 1);
	static trv2::RectI GetTileRect(const trv2::RectI& worldRect);
	static trv2::RectI GetTileSectionRect(const trv2::RectI& worldRect);

	static constexpr int TILE_SIZE = 16;
	static constexpr int TILE_SECTION_SIZE = 32;
	static constexpr int TILE_OFF_SCREEN = 32;
private:
	static constexpr int TILE_SECTION_CACHE_SIZE = 16;

	std::shared_ptr<TileSection> _cachedSections[TILE_SECTION_CACHE_SIZE][TILE_SECTION_CACHE_SIZE];

	TileSection* getTileSection(glm::ivec2 sectionPos);
	const TileSection* checkInCache(glm::ivec2 sectionPos) const;
};