#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <TREngine/Core.h>
#include <TRGame/TRGame_Interfaces.h>

class WorldResource
{
public:
	WorldResource();
	~WorldResource();

private:
	std::unique_ptr<trv2::Texture2D* []> _tileTextures;
	std::unique_ptr<glm::vec3 []> _tileMapColors;
	std::unique_ptr<bool []> _tileSolid;
};