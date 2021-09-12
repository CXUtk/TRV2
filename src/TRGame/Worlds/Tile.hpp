#pragma once
#include <glm/glm.hpp>
class Tile
{
public:
	Tile() = default;
	Tile(glm::vec3 color) :_color(color) {}
	~Tile() {}

	glm::vec3 GetColor() const { return _color; }
	void SetColor(glm::vec3 value) { _color = value; }
private:
	glm::vec3 _color;

};


struct TileGenLayout
{
	float v[5];
};