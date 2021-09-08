#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Core.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLTexture2D
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

	~OpenGLTexture2D();

	ITextureHandle GetHandle() const { return _handle; }

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

private:
	ITextureHandle _handle;
	int _width, _height;
	IGraphicsDevice* _graphicsDevice;
};
TRV2_NAMESPACE_END