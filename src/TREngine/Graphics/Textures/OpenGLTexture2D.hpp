#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Graphics/Graphics_Interfaces.hpp>
#include <Core.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLTexture2D
{
public:
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

	/**
	 * @brief 
	 * @param device 
	 * @param width 
	 * @param height 
	 * @param data 
	 * @param internalFormat 
	 * @param srcFormat 
	 * @param parameters 
	*/
	OpenGLTexture2D(IGraphicsDevice* device, int width, int height, unsigned char* data,
		PixelFormat internalFormat, PixelFormat srcFormat, EngineDataType dataType, const TextureParameters& parameters);

	~OpenGLTexture2D();

	ITextureHandle GetHandle() const { return _handle; }

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

private:
	ITextureHandle _handle;
	int _width, _height;
	IGraphicsDevice* _graphicsDevice;
	TextureParameters _parameters;

	void genNewTexture2D(IGraphicsDevice* device, int width, int height, unsigned char* data,
		PixelFormat internalFormat, PixelFormat srcFormat, EngineDataType dataType, const TextureParameters& parameters);
};
TRV2_NAMESPACE_END