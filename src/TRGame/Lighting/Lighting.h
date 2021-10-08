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


struct Light
{
	glm::vec2 Position;
	glm::vec3 Color;
	int Radius;
};

struct LightNode
{
	glm::i16vec2 Pos;
	float Luminance;
	int k;

	bool operator<(const LightNode& B) const
	{
		return this->Luminance < B.Luminance;
	}
};

struct Segment;

class Lighting
{
public:
	Lighting();
	~Lighting();

	void SetGameWorld(GameWorld* gameWorld) { _gameWorld = gameWorld; }
	void ClearLights();
	void AddLight(const Light& light);
	void AddDirectionalLight(const Light& light);

	void CalculateLight(const trv2::RectI& tileRectCalc, const trv2::RectI& tileRectScreen);
	void DrawLightMap(trv2::SpriteRenderer* renderer, const glm::mat4& projection);
	void DrawDirectionalTriangles(const glm::mat4& worldProjection);
	float GetLight(glm::ivec2 coord);
private:
	static constexpr int MAX_TILES = 256 * 256;

	float _colors[3][MAX_TILES];
	float _luminances[3][MAX_TILES];
	bool _visited[3][MAX_TILES];
	int _cachedTileTypes[MAX_TILES];
	std::priority_queue<LightNode> _lightQ[3];


	GameWorld* _gameWorld = nullptr;
	std::vector<Light> _lights;
	std::vector<Light> _directionalLights;
	trv2::RectI _tileRect{};
	trv2::RectI _tileRectScreen{};


	trv2::IVertexBufferHandle _vao, _vbo;

	int getBlockId(glm::ivec2 localCoord);
	bool isValidCoord(glm::ivec2 worldCoord);
	bool isValidCoordCached(glm::ivec2 worldCoord);
	bool canTilePropagateLight(glm::ivec2 worldCoord);
	float calculateLuminance(glm::ivec2 worldCoord, int dir, float curLuminance);
	int getCachedTileType(glm::ivec2 worldCoord) const;

	/**
	* @brief Add segments and keypoints according to their angle to the center
	* @param rect
	* @param segments
	* @param keypoints
	* @param center
	*/
	void addShadowSegments(const trv2::RectI& worldTileRect, std::vector<Segment>& segments, const glm::vec2& center);

	void calculateOneChannel(const std::vector<Light>& lights, int channel);
	void calculateDirectionLight(const std::vector<Light>& dLights);
};