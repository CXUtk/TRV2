﻿#pragma once
#include "WorldMap.h"

#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>
#include <TRGame/TRGame_Interfaces.h>

class TileSection
{
public:
	TileSection(glm::ivec2 tileStart, glm::ivec2 tileSize);
	~TileSection();
	Tile& GetTile(glm::ivec2 pos);
	const Tile& GetTile(glm::ivec2 pos) const;
	void SetTile(glm::ivec2 pos, const Tile& tile);

	glm::ivec2 GetSectionSize() const { return _sectionSize; }
	glm::ivec2 GetSectionStartPos() const { return _sectionStart; }
	bool Intersects(const trv2::RectI& tileRect) const;

	void RenderSection(const glm::mat4& projection, trv2::SpriteRenderer* renderer, trv2::RenderTarget2D* renderTarget);

	trv2::Texture2D* GetMapTexture() const { return _sectionMap->GetTexture(); }

private:
	std::unique_ptr<Tile[]> _tiles;
	std::unique_ptr<TileGenLayout[]> _worldGenLayouts;
	std::unique_ptr<WorldMap> _sectionMap;

	std::unique_ptr<trv2::RenderTarget2D> _cacheRenderTargetTiles;
	std::unique_ptr<trv2::RenderTarget2D> _cacheRenderTargetWalls;

	glm::ivec2 _sectionStart;
	glm::ivec2 _sectionSize;

	bool _isDirty = true;

	void reDrawCache(trv2::RenderTarget2D* renderTarget);
};