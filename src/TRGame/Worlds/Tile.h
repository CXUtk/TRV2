#pragma once
#include <glm/glm.hpp>
#include <bitset>
#include <TRGame/TRGame_Interfaces.h>
#include <TREngine/Core.h>

struct TileGenLayout
{
	float v[5];
};

struct Tile
{
	uint Type : 16;
	uint Wall : 16;
	uint TileFrameX : 16;
	uint TileFrameY : 16;
	uint Life : 8;
	bool Active : 1;

	Tile() = default;
	Tile(int type) : Type(type) {}
	~Tile() = default;

	bool IsEmpty() const { return Type == 0 && Wall == 0; }
	bool IsAir() const { return Type == 0; }
};

struct WallObjectData
{
	bool UseShader = false;
	trv2::ShaderProgram* Shader = nullptr;
	trv2::Texture2D* Texture = nullptr;
};

struct TileObjectData
{
	bool Solid = false;
	bool BlockLight = false;
	trv2::Texture2D* Texture = nullptr;
	glm::vec3 MapColor{};
};