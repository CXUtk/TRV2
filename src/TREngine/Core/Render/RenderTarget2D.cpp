#include "RenderTarget2D.h"
#include <Core/Render/Texture2D.h>

TRV2_NAMESPACE_BEGIN

RenderTarget2D::RenderTarget2D(IGraphicsResourceManager* resourceManager, glm::ivec2 size, const TextureParameters& parameter) :
	IGraphicsResource(resourceManager), _width(size.x), _height(size.y), _handle(-1)
{
	_renderTexture = std::make_shared<Texture2D>(resourceManager, _width, _height, nullptr, PixelFormat::RGB, PixelFormat::RGB, EngineDataType::UNSIGNED_BYTE, parameter);
	_handle = resourceManager->CreateRenderTarget2D(trv2::ptr(_renderTexture), _width, _height);
}

RenderTarget2D::~RenderTarget2D()
{
	if (_handle != -1)
		_resourceManager->DeleteTexture2D(_handle);
}
TRV2_NAMESPACE_END