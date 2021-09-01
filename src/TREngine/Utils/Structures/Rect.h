#pragma once
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
struct Rect
{
	glm::vec2 Position;
	glm::vec2 Size;

	Rect() = default;
	Rect(glm::vec2 pos, glm::vec2 size) : Position(pos), Size(size) {}
};


struct RectI
{
	glm::ivec2 Position;
	glm::ivec2 Size;

	RectI() = default;
	RectI(glm::ivec2 pos, glm::ivec2 size) : Position(pos), Size(size) {}
};
TRV2_NAMESPACE_END