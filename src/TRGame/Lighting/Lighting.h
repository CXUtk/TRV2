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

	void CalculateLight(const trv2::RectI& tileRectScreen);
	void DrawLightMap(trv2::SpriteRenderer* renderer, const glm::mat4& projection);
	float GetLight(glm::ivec2 coord);
private:

	GameWorld* _gameWorld = nullptr;
	std::vector<Light> _lights;
	trv2::RectI _tileRect{};
	trv2::RectI _tileRectScreen{};

	int getBlockId(glm::ivec2 localCoord);
	bool isValidCoord(glm::ivec2 worldCoord);
	bool isValidCoordCached(glm::ivec2 worldCoord);
	bool canTilePropagateLight(glm::ivec2 worldCoord);
	float calculateLuminance(glm::ivec2 worldCoord, int dir, float curLuminance);

	void calculateOneChannel(const std::vector<Light>& lights, int channel);
};