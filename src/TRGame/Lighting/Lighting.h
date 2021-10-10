#pragma once
#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>
#include <TREngine/Graphics/Graphics_Interfaces.h>

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <vector>
#include <queue>

#include "LightCalculator/LightCalculator.h"
class LightCommon;

class Lighting
{
public:
	Lighting();
	~Lighting();

	void ClearLights();
	void AddNormalLight(const Light& light);
	void AddDirectionalLight(const Light& light);

	void CalculateLight(const trv2::RectI& tileRectCalc, const trv2::RectI& tileRectScreen);
	void DrawLightMap(trv2::SpriteRenderer* renderer, const glm::mat4& projection);
	void DrawDirectionalTriangles(const glm::mat4& worldProjection);
	float GetLight(glm::ivec2 coord);
private:
	std::unique_ptr<LightCommon> _lightCommonData;

	std::unique_ptr<BFSLightCalculator> _bfsCalculator;
	std::unique_ptr<DirectionalLightCalculator> _directionCalculator;



	///**
	//* @brief Add segments and keypoints according to their angle to the center
	//* @param rect
	//* @param segments
	//* @param keypoints
	//* @param center
	//*/
	//void addShadowSegments(const trv2::RectI& worldTileRect, std::vector<Segment>& segments, const glm::vec2& center);

	//void calculateOneChannel(const std::vector<Light>& lights, int channel);
	//void calculateDirectionLight(const std::vector<Light>& dLights);
};