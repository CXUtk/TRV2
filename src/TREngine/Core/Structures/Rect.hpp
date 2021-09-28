#pragma once
#include <Core.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <functional>

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
	glm::vec<2, T> Center() const { return Position + (Size / (T)2); }

	void ForEach(std::function<void(glm::vec<2, T>)> func)
	{
		for (T y = Position.y; y < Position.y + Size.y; y++)
		{
			for (T x = Position.x; x < Position.x + Size.x; x++)
			{
				func(glm::vec<2, T>(x, y));
			}
		}
	}

};

using Rectf = Rect2D<float>;
using RectI = Rect2D<int>;

template<typename T, typename V>
inline bool RectIntersects(const Rect2D<T>& A, const Rect2D<V>& B)
{
	return std::max(A.Position.x, B.Position.x) < std::min(A.Position.x + A.Size.x, B.Position.x + B.Size.x)
		&& std::max(A.Position.y, B.Position.y) < std::min(A.Position.y + A.Size.y, B.Position.y + B.Size.y);
}
TRV2_NAMESPACE_END