#pragma once
#include <Core.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <Core/Structures/Rect.hpp>

TRV2_NAMESPACE_BEGIN

enum class CollisionSide
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};
/**
 * @brief 
 * @tparam T 
 * @param oldRect 
 * @param newRect 
 * @param velocity 
 * @return 
*/
template<typename T>
inline CollisionSide GetCollisionSide(const Rect2D<T>& oldRect, glm::vec<2, T> velocity, const Rect2D<T>& targetRect)
{
	// Need to make sure oldRect does not intersects with target
	// And newRect must intersects with target
	if (velocity.x > 0)
	{
		auto distX = targetRect.Position.x - (oldRect.Position.x + oldRect.Size.x);
		if (distX > 0)
		{
			double t = (double)distX / velocity.x;
			double newY1 = oldRect.Position.y + velocity.y * t;
			double newY2 = newY1 + oldRect.Size.y;

			// If Y axis collide, then it's left side collide
			if (std::max(newY1, (double)targetRect.Position.y) <= std::min(newY2, (double)(targetRect.Position.y + targetRect.Size.y)))
			{
				return CollisionSide::LEFT;
			}
			else
			{
				return velocity.y > 0 ? CollisionSide::BOTTOM : CollisionSide::TOP;
			}
		}
		else
		{
			return velocity.y > 0 ? CollisionSide::BOTTOM :  CollisionSide::TOP;
		}
	}
	else
	{
		auto distX = oldRect.Position.x - (targetRect.Position.x + targetRect.Size.x);
		if (distX > 0)
		{
			double t = (double)distX / velocity.x;
			if (std::isnan(t)) return CollisionSide::RIGHT;

			double newY1 = oldRect.Position.y + velocity.y * t;
			double newY2 = newY1 + oldRect.Size.y;

			// If Y axis collide, then it's right side collide
			if (std::max(newY1, (double)targetRect.Position.y) <= std::min(newY2, (double)(targetRect.Position.y + targetRect.Size.y)))
			{
				return CollisionSide::RIGHT;
			}
			else
			{
				return velocity.y > 0 ? CollisionSide::BOTTOM : CollisionSide::TOP;
			}
		}
		else
		{
			return velocity.y > 0 ? CollisionSide::BOTTOM : CollisionSide::TOP;
		}
	}

	//if (velocity.x > 0)
	//{
	//	auto distX = targetRect.x - (oldRect.Position.x + oldRect.Size.x);
	//	if (dist > 0)
	//	{
	//		double t = (double)distX / velocity.x;
	//		double newY1 = oldRect.Position.y + velocity.y * t;
	//		double newY2 = newY1 + oldRect.Size.y;

	//		// If Y axis collide, then it's left side collide
	//		if (std::max(newY1, targetRect.Position.y) <= std::min(newY2, targetRect.Position.y + targetRect.Size.y))
	//		{
	//			return CollisionSide::LEFT;
	//		}
	//	}
	//}
	//else
	//{
	//	auto distX = targetRect.x + targetRect.Size.x - oldRect.Position.x;
	//	if (dist > 0)
	//	{
	//		double t = (double)distX / velocity.x;
	//		if (std::isnan(t)) return CollisionSide::RIGHT;

	//		double newY1 = oldRect.Position.y + velocity.y * t;
	//		double newY2 = newY1 + oldRect.Size.y;

	//		// If Y axis collide, then it's right side collide
	//		if (std::max(newY1, targetRect.Position.y) <= std::min(newY2, targetRect.Position.y + targetRect.Size.y))
	//		{
	//			return CollisionSide::RIGHT;
	//		}
	//	}
	//}
}
TRV2_NAMESPACE_END