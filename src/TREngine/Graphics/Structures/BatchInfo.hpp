#pragma once
#include <Core.hpp>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
struct BatchInfo
{
	glm::vec2 Position, Size;
	glm::vec4 Color;
	BatchInfo() = default;
	BatchInfo(glm::vec2 pos, glm::vec2 size, const glm::vec4& color) : Position(pos), Size(size), Color(color) {}
};

enum class SpriteSortMode
{
	Deferred,
	FrontToBack,
	BackToFront,
	Texture,
};

struct BatchSettings
{
	SpriteSortMode SpriteSortMode;
};

struct BatchState
{
	glm::mat4 WorldTransform;
	bool IsBatchBegin;
	BatchSettings Settings;
	BatchState() = default;
};


struct BatchVertex2D
{
	glm::vec2 Position;
	glm::vec2 TextureCoords;
	glm::vec4 Color;
	float TextureIndex;

	BatchVertex2D() = default;
	BatchVertex2D(glm::vec2 pos, glm::vec2 texCoords, const glm::vec4& color) : Position(pos),
		TextureCoords(texCoords), Color(color), TextureIndex(0.f) {}
};
TRV2_NAMESPACE_END