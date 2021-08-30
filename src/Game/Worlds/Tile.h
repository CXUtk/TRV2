#pragma once

#include <TRV2.h>
#include <memory>

class Tile
{
public:
	Tile() = default;
	Tile(int type) :_type(type) {}
	~Tile() {}

	ReadonlyProperty(int, Type, type);
private:

};