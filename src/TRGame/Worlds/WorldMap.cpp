#include "WorldMap.h"
#include <TREngine/Core/Render/render.h>
#include <TRGame/TRGame.hpp>
#include <TREngine/Engine.h>

WorldMap::WorldMap(glm::ivec2 size) : _size(size)
{
	_cacheData = std::make_unique<Color_byte3[]>(size.x * size.y);
	trv2::TextureParameters texPara{};
	_mapTexture = std::make_unique<trv2::Texture2D>(trv2::Engine::GetInstance()->GetGraphicsResourceManager(),
		size, texPara, trv2::PixelFormat::RGB, trv2::EngineDataType::UNSIGNED_BYTE, nullptr);
}

WorldMap::~WorldMap()
{}

void WorldMap::SetColor(glm::ivec2 pos, glm::vec3 color)
{
	_isDirty = true;
	_cacheData[pos.y * _size.x + pos.x] = vec3ToColorByte3(color);
}

trv2::Texture2D* WorldMap::GetTexture()
{
	if (_isDirty)
	{
		trv2::TextureParameters texPara{};
		_mapTexture->ChangeBuffer(_size, texPara, trv2::PixelFormat::RGB, 
			trv2::EngineDataType::UNSIGNED_BYTE, _cacheData.get());
	}
	return trv2::ptr(_mapTexture);
}
