#pragma once

#include <string>

#include <glm/glm.hpp>
#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class RenderTarget2D : public IGraphicsResource
{
public:
	
	RenderTarget2D(IGraphicsResourceManager* resourceManager, glm::ivec2 size, const TextureParameters& parameter);
	~RenderTarget2D();

	IRenderTarget2DHandle GetHandle() const { return _handle; }
	Texture2D* GetTexture2D() const { return trv2::ptr(_renderTexture); }

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

private:
	IRenderTarget2DHandle _handle;

	int _width, _height;
	std::shared_ptr<Texture2D> _renderTexture;
};
TRV2_NAMESPACE_END