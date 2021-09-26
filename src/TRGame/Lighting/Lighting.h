#pragma once
#include <TRGame/TRGame_Interfaces.h>
#include <glm/glm.hpp>
#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>
#include <vector>

struct Light
{
	glm::vec2 Position;
	glm::vec3 Color;
	int Radius;
};

class Lighting
{
public:
	Lighting();
	~Lighting();

	void SetGameWorld(GameWorld* gameWorld) { _gameWorld = gameWorld; }
	void ClearLights();
	void AddLight(const Light& light);

	void CalculateLight(trv2::SpriteRenderer* renderer, const glm::mat4& projection, const trv2::RectI& tileRect);
	float GetLight(glm::ivec2 coord);
private:

	GameWorld* _gameWorld = nullptr;
	std::vector<Light> _lights;

	int getBlockId(glm::ivec2 localCoord);
	bool isValidCoord(glm::ivec2 worldCoord);
	bool isValidCoordCached(glm::ivec2 worldCoord);
	bool canTilePropagateLight(glm::ivec2 worldCoord);
	float calculateDistance(glm::ivec2 worldCoord, int dir, float curDist);

	void calculateOneLight(const Light& light);
};