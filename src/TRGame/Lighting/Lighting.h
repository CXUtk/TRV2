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

	void CalculateLight();
	static float GetLight(glm::ivec2 pos);
private:

};