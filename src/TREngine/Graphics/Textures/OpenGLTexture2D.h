#pragma once

#include <TREngine_Interfaces.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Graphics/Graphics_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class OpenGLTexture2D : public ITexture2D
{
public:
	/**
	 * @brief *Don't use it* If you want, please ensure it's a std::move on handle
	 * @param device 
	 * @param handle 
	*/
	OpenGLTexture2D(IGraphicsDevice* device, ITextureHandle handle);

	/**
	 * @brief Creates a texture 2d object by accepting data in memory
	 * @param device The graphics device that will generate this texture
	 * @param width Width of the image
	 * @param height Height of the image
	 * @param data Data of the image
	*/
	OpenGLTexture2D(IGraphicsDevice* device, int width, int height, unsigned char* data);

	/**
	 * @brief Creates a texture 2d object from file
	 * @param device The graphics device that will generate this texture
	 * @param fileName Name of the image file
	*/
	OpenGLTexture2D(IGraphicsDevice* device, const std::string& fileName);

	~OpenGLTexture2D() override;

	ITextureHandle GetHandle() const override { return _handle; }

	int GetWidth() const override { return _width; }
	int GetHeight() const override { return _height; }

private:
	ITextureHandle _handle;
	int _width, _height;
	IGraphicsDevice* _graphicsDevice;
};
TRV2_NAMESPACE_END