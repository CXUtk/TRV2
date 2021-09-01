#pragma once
#include <TREngine_Interfaces.h>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
struct BatchInfo
{
	glm::vec2 Position, Size;
	glm::vec4 Color;
	BatchInfo() = default;
	BatchInfo(glm::vec2 pos, glm::vec2 size, const glm::vec4& color) : Position(pos), Size(size), Color(color) {}
};

struct BatchState
{
	glm::mat4 WorldTransform;
	BatchState() = default;
	BatchState(const glm::mat4& transform) : WorldTransform(transform) {}
};

struct Vertex2D
{
	glm::vec2 Position;
	glm::vec2 TextureCoords;
	glm::vec4 Color;
	Vertex2D() = default;
	Vertex2D(glm::vec2 pos, glm::vec2 texCoords, const glm::vec4& color) : Position(pos),
		TextureCoords(texCoords), Color(color) {}
};
TRV2_NAMESPACE_END