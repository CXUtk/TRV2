#pragma once
#include <TRGame/TRGame_Interfaces.h>
#include <glm/glm.hpp>
#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>

class Lighting
{
public:
	Lighting();
	~Lighting();

	static void CalculateLight(trv2::SpriteRenderer* renderer, const glm::mat4& projection, 
		const GameWorld* world, const trv2::Rect2D<float>& screenRect);

private:
};