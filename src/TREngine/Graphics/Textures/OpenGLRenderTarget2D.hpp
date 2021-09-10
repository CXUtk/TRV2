#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Graphics/Graphics_Interfaces.hpp>
#include <Graphics/OpenGLGraphicsAPI.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLRenderTarget2D
{
public:
	
	OpenGLRenderTarget2D(OpenGLGraphicsDevice* device, int width, int height);
	~OpenGLRenderTarget2D();

	IRenderTarget2DHandle GetHandle() const { return _handle; }
	OpenGLTexture2D* GetTexture2D() const { return _renderTexture.get(); }

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

private:
	IRenderTarget2DHandle _handle;

	int _width, _height;
	OpenGLGraphicsDevice* _graphicsDevice;
	std::shared_ptr<OpenGLTexture2D> _renderTexture;
};
TRV2_NAMESPACE_END