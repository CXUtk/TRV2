#pragma once
#include <glm/glm.hpp>
#include <bitset>

struct TileGenLayout
{
	float v[5];
};


class Tile
{
public:
	Tile() = default;
	Tile(glm::vec3 color) :_color(color) {}
	~Tile() {}

	bool IsSolid() const { return _solid; }
	bool IsEmpty() const { return _empty; }

	void SetSolid(bool value) { _solid = value; }
	void SetEmpty(bool value) { _empty = value; }

	glm::vec3 GetColor() const { return _color; }
	void SetColor(glm::vec3 value) { _color = value; }
private:
	glm::vec3 _color{};

	bool _empty = false;
	bool _solid = false;
};

