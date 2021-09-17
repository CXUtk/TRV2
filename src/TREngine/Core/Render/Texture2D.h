#pragma once
#include <string>
#include <glm/glm.hpp>

#include <Graphics/Graphics_Interfaces.h>
#include <Core.h>

TRV2_NAMESPACE_BEGIN
class Texture2D : public IGraphicsResource
{
public:
	/**
	 * @brief Creates a texture 2d object by accepting data in memory
	 * @param device The graphics device that will generate this texture
	 * @param width Width of the image
	 * @param height Height of the image
	 * @param data Data of the image
	*/
	Texture2D(IGraphicsResourceManager* resourceManager, int width, int height, const void* data);

	/**
	 * @brief Creates a texture 2d object from file
	 * @param device The graphics device that will generate this texture
	 * @param fileName Name of the image file
	*/
	Texture2D(IGraphicsResourceManager* resourceManager, const std::string& fileName);

	/**
	 * @brief Creates a texture 2d object with various parameters
	 * @param device The graphics device that will generate this texture
	 * @param width Width of the image 
	 * @param height Height of the image
	 * @param data Data of the image
	 * @param internalFormat Internal format of this texture
	 * @param srcFormat Format of the source image
	 * @param dataType Data type used in source image
	 * @param parameters Texture parameters in the generating process
	*/
	Texture2D(IGraphicsResourceManager* resourceManager, int width, int height, const void* data,
		PixelFormat internalFormat, PixelFormat srcFormat, EngineDataType dataType, const TextureParameters& parameters);

	~Texture2D();

	ITextureHandle GetHandle() const { return _handle; }
	void Resize(glm::ivec2 size);

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

private:
	ITextureHandle _handle;
	int _width, _height;
	TextureParameters _parameters;
};
TRV2_NAMESPACE_END