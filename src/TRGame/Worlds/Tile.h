#pragma once
#include <glm/glm.hpp>
#include <bitset>

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
	bool Solid : 1;

	Tile() = default;
	Tile(int type) : Type(type) {}
	~Tile() = default;

	bool IsSolid() const { return Solid; }
	bool IsEmpty() const { return Type == 0 && Wall == 0; }
	bool IsAir() const { return Type == 0; }
	void SetSolid(bool value) { Solid = value; }
};

struct TileObjectData
{
	bool Solid;
	bool BlockLight;
};