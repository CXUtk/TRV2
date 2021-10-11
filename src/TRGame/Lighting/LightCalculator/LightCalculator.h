#pragma once
#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/Core.h>
#include <glm/glm.hpp>

struct Light
{
	glm::vec2 Position;
	glm::vec3 Color;
	int Radius;
};

class BFSLightCalculator;
class DirectionalLightCalculator;

class LightCalculator
{
public:
	virtual ~LightCalculator() = 0 {}
	virtual void AddLight(const Light& light) = 0;
	virtual void ClearLights() = 0;
	virtual void Calculate() = 0;
};