#include "RenderTarget2D.h"
#include <Core/Render/Texture2D.h>

TRV2_NAMESPACE_BEGIN

RenderTarget2D::RenderTarget2D(IGraphicsResourceManager* resourceManager, glm::ivec2 size, const TextureParameters& parameter) :
	IGraphicsResource(resourceManager), _handle(-1), _parameters(parameter)
{
	_renderTexture = std::make_shared<Texture2D>(resourceManager, size, 
		parameter, PixelFormat::RGB, EngineDataType::UNSIGNED_BYTE, nullptr);
	_handle = resourceManager->CreateRenderTarget2D(trv2::ptr(_renderTexture), size);
}

RenderTarget2D::~RenderTarget2D()
{
	_renderTexture.reset();
	if (_handle != -1)
	{
		_resourceManager->DeleteRenderTarget2D(_handle);
	}
}
void RenderTarget2D::Resize(glm::ivec2 size)
{
	_renderTexture->ChangeBuffer(size, _parameters, PixelFormat::RGB, EngineDataType::UNSIGNED_BYTE, nullptr);
}
glm::ivec2 RenderTarget2D::GetSize() const
{
	return _renderTexture->GetSize(); 
}
TRV2_NAMESPACE_END