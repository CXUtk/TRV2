#pragma once
#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>


struct TileCacheInfo
{
	int Type;
};


struct Triangle;

class LightCommon
{
public:
	// Constants
	static constexpr int dX[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
	static constexpr int dY[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };

	static constexpr int dX4[4] = { 1, -1, 0, 0 };
	static constexpr int dY4[4] = { 0, 0, 1, -1 };
	static constexpr float MIN_LUMINANCE = 0.f;
	static constexpr float DECREASE = 0.0625;
	static constexpr float DECREASE_DIAG = 0.0883;
	static constexpr float distA[8] = { DECREASE, DECREASE, DECREASE, DECREASE,
			DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG, DECREASE_DIAG };

	static constexpr float EPS = 1e-4;

	static constexpr float MAXDIST = 16.f;
	static constexpr int K = 1;
	static constexpr int MAX_TILES = 256 * 256;
	static constexpr int CHANNELS = 3;

	// Data
	GameWorld* GameWorld = nullptr;

	float TileColors[CHANNELS][MAX_TILES] = { 0 };
	TileCacheInfo CachedTile[MAX_TILES] = { 0 };

	/**
	 * @brief Tile rectangle in calculation range
	*/
	trv2::RectI TileRectWorld{};

	/**
	 * @brief Tile rectangle in screen range
	*/
	trv2::RectI TileRectScreen{};
	trv2::RectI SectionRect{};


	int GetBlockId(glm::ivec2 localCoord) const;
	bool IsTileCoordInRange(glm::ivec2 worldCoord) const;
	bool CanTilePropagateLight(glm::ivec2 worldCoord) const;
	const TileCacheInfo& GetCachedTile(glm::ivec2 worldCoord) const;
};


