#pragma once
#include <memory>

#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>


class GameWorld
{
public:
	GameWorld(int width, int height);
	~GameWorld();

	

	trv2::RectI GetTileRect(const trv2::Rectf& worldRect) const;

	void RenderWorld(const glm::mat4& projection, trv2::SpriteRenderer* renderer, const trv2::Rect2D<float>& renderRect);

	trv2::Texture2D* GetMapTexture();


	static constexpr int TILE_SIZE = 16;
	static glm::ivec2 GetLowerWorldCoord(glm::vec2 pos);
	static glm::ivec2 GetUpperWorldCoord(glm::vec2 pos);

private:



};