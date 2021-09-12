#include "RenderTarget2D.h"
#include <Core/Render/Texture2D.h>

TRV2_NAMESPACE_BEGIN

RenderTarget2D::RenderTarget2D(IGraphicsResourceManager* resourceManager, int width, int height) :
	IGraphicsResource(resourceManager), _width(width), _height(height), _handle(-1)
{
	_renderTexture = std::make_shared<Texture2D>(resourceManager, width, height, nullptr, PixelFormat::RGB, PixelFormat::RGB, EngineDataType::UNSIGNED_BYTE, TextureParameters());
	_handle = resourceManager->CreateRenderTarget2D(trv2::ptr(_renderTexture), width, height);
}

RenderTarget2D::~RenderTarget2D()
{
	if (_handle != -1)
		_resourceManager->DeleteShaderProgram(_handle);
}
TRV2_NAMESPACE_END