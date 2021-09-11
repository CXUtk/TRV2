#pragma once
#include <Core.h>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
template<typename T>
struct Rect2D
{
	glm::vec<2, T> Position;
	glm::vec<2, T> Size;

	Rect2D() = default;
	Rect2D(glm::vec<2, T> pos, glm::vec<2, T> size) : Position(pos), Size(size) {}

	glm::vec<2, T> BottomLeft() const { return Position; }
	glm::vec<2, T> BottomRight() const { return Position + glm::vec<2, T>(Size.x, 0); }
	glm::vec<2, T> TopLeft() const { return Position + glm::vec<2, T>(0, Size.y); }
	glm::vec<2, T> TopRight() const { return Position + Size; }
};
TRV2_NAMESPACE_END