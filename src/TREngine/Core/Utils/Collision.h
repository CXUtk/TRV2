#pragma once
#include <Core.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include <Core/Structures/Rect.hpp>

TRV2_NAMESPACE_BEGIN

constexpr float EPS = 1e-5;

enum class CollisionSide
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};


struct Interval
{
	bool horizontal;
	float V;
	float L, R;
};

inline bool IntervalIntersects(const Interval& A, const Interval& B)
{
	return std::max(A.L, B.L) < std::min(A.R, B.R);
}

inline std::vector<Interval> GetCollidingSegments(const Rectf& rect, glm::vec2 velocity)
{
	std::vector<Interval> segments;
	if (velocity.x > 0)
	{
		// 右碰撞段
		segments.push_back({ false, rect.Position.x + rect.Size.x, rect.Position.y, rect.Position.y + rect.Size.y });
	}
	else if (velocity.x < 0)
	{
		// 左碰撞段
		segments.push_back({ false, rect.Position.x, rect.Position.y, rect.Position.y + rect.Size.y });
	}
	// 如果速度x等于0，那么不存在左右碰撞段

	if (velocity.y > 0)
	{
		// 上碰撞段
		segments.push_back({ true, rect.Position.y + rect.Size.y, rect.Position.x, rect.Position.x + rect.Size.x });
	}
	else if(velocity.y < 0)
	{
		// 下碰撞段
		segments.push_back({ true, rect.Position.y, rect.Position.x, rect.Position.x + rect.Size.x });
	}
	// 如果速度y等于0，那么不存在上下碰撞段
	return segments;
}

inline std::vector<Interval> GetCollidingSegmentsRev(const Rectf& rect, glm::vec2 velocity)
{
	std::vector<Interval> segments;
	if (velocity.x > 0)
	{
		// 左碰撞段
		segments.push_back({ false, rect.Position.x, rect.Position.y, rect.Position.y + rect.Size.y });
	}
	else if (velocity.x < 0)
	{
		// 右碰撞段
		segments.push_back({ false, rect.Position.x + rect.Size.x, rect.Position.y, rect.Position.y + rect.Size.y });
	}
	// 如果速度x等于0，那么不存在左右碰撞段

	if (velocity.y > 0)
	{
		// 下碰撞段
		segments.push_back({ true, rect.Position.y, rect.Position.x, rect.Position.x + rect.Size.x });
	}
	else if(velocity.y < 0)
	{
		// 上碰撞段
		segments.push_back({ true, rect.Position.y + rect.Size.y, rect.Position.x, rect.Position.x + rect.Size.x });
	}
	// 如果速度y等于0，那么不存在上下碰撞段
	return segments;
}

inline float GetNearestCollisionTime(const Interval& subject, glm::vec2 velocity, const std::vector<Interval>& intervals)
{
	double minimalTime = std::numeric_limits<double>::infinity();
	for (auto& inv : intervals)
	{
		if (subject.horizontal)
		{
			double distY = inv.V - subject.V;
			double t = distY / velocity.y;
			if (t < 0) continue;

			double moveX = velocity.x * t;
			// If X collides
			if (std::max(subject.L + moveX, (double)inv.L) < std::min(subject.R + moveX, (double)inv.R))
			{
				if (t < minimalTime)
				{
					minimalTime = t;
				}
			}
		}
		else
		{
			double distX = inv.V - subject.V;
			double t = distX / velocity.x;
			if (t < 0) continue;

			double moveY = velocity.y * t;
			// If X collides
			if (std::max(subject.L + moveY, (double)inv.L) < std::min(subject.R + moveY, (double)inv.R))
			{
				if (t < minimalTime)
				{
					minimalTime = t;
				}
			}
		}
	}
	return minimalTime;
}

inline CollisionSide GetCollisionSide(const Rectf& oldRect, glm::vec2 velocity, const Rectf& targetRect)
{
	// Need to make sure oldRect does not intersects with target
	// And newRect must intersects with target
	if (velocity.x > 0)
	{
		auto distX = targetRect.Position.x - (oldRect.Position.x + oldRect.Size.x);
		if (distX >= 0)
		{
			double t = (double)distX / velocity.x;
			double newY1 = oldRect.Position.y + velocity.y * t;
			double newY2 = newY1 + oldRect.Size.y;

			// If Y axis collide, then it's left side collide
			if (std::max(newY1, (double)targetRect.Position.y) < std::min(newY2, (double)(targetRect.Position.y + targetRect.Size.y)))
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
		if (distX >= 0)
		{
			double t = (double)distX / velocity.x;
			if (std::isnan(t)) return CollisionSide::RIGHT;

			double newY1 = oldRect.Position.y + velocity.y * t;
			double newY2 = newY1 + oldRect.Size.y;

			// If Y axis collide, then it's right side collide
			if (std::max(newY1, (double)targetRect.Position.y) < std::min(newY2, (double)(targetRect.Position.y + targetRect.Size.y)))
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