#pragma once
class Tile
{
public:
	Tile() = default;
	Tile(int type) :_type(type) {}
	~Tile() {}

	int GetType() const { return _type; }
private:
	int _type;
};
