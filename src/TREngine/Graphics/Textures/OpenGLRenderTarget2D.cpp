#include "OpenGLRenderTarget2D.hpp"
#include <Graphics/Textures/OpenGLTexture2D.hpp>

TRV2_NAMESPACE_BEGIN
OpenGLRenderTarget2D::OpenGLRenderTarget2D(OpenGLGraphicsDevice* device, int width, int height)
{
	_graphicsDevice = device;
	_width = width;
	_height = height;
	
	glGenFramebuffers(1, &_handle);

	glBindFramebuffer(GL_FRAMEBUFFER, _handle);

	_renderTexture = std::make_shared<OpenGLTexture2D>(device, width, height, nullptr, PixelFormat::RGB, PixelFormat::RGB, EngineDataType::UNSIGNED_BYTE, TextureParameters());

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture->GetHandle(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::exception("Framebuffer not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLRenderTarget2D::~OpenGLRenderTarget2D()
{
	glDeleteFramebuffers(1, &_handle);
}
TRV2_NAMESPACE_END