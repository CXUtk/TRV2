#pragma once

#include <TREngine/Core.h>
#include <TREngine/Core/Structures/Rect.hpp>
#include <TRGame/TRGame_Interfaces.h>

class TileSection
{
public:
	TileSection(glm::ivec2 start, glm::ivec2 size);
	~TileSection();
	Tile& GetTile(glm::ivec2 pos);
	const Tile& GetTile(glm::ivec2 pos) const;
	void SetTile(glm::ivec2 pos, const Tile& tile);

	glm::ivec2 GetSectionSize() const { return _sectionSize; }
	glm::ivec2 GetSectionStartPos() const { return _sectionStart; }

private:
	std::unique_ptr<Tile[]> _tiles;
	std::unique_ptr<TileGenLayout[]> _worldGenLayouts;
	std::unique_ptr<WorldMap> _sectionMap;

	glm::ivec2 _sectionStart;
	glm::ivec2 _sectionSize;

	bool _isDirty = false;
};