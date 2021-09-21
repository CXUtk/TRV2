#pragma once
#include <glm/glm.hpp>
#include <bitset>
#include <TREngine/Core.h>

struct Color_byte3
{
	byte8 R;
	byte8 G;
	byte8 B;
};

class WorldMap
{
public:
	WorldMap(glm::ivec2 size);
	~WorldMap();

	void SetColor(glm::ivec2 pos, glm::vec3 color);
	trv2::Texture2D* GetTexture();
	
private:
	bool _isDirty = true;
	glm::ivec2 _size{};

	std::unique_ptr<Color_byte3[]> _cacheData;
	std::unique_ptr<trv2::Texture2D> _mapTexture;
};

inline Color_byte3 vec3ToColorByte3(const glm::vec3& color)
{
	return { (byte8)(std::min(OneMinusEpsilon, color.r) * 256),
		(byte8)(std::min(OneMinusEpsilon, color.g) * 256),
		(byte8)(std::min(OneMinusEpsilon, color.b) * 256) };
}
