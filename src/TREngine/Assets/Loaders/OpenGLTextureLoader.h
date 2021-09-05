#pragma once
#include <memory>
#include <string>
#include <TREngine_Interfaces.h>
#include <Graphics/Textures/OpenGLTexture2D.h>

TRV2_NAMESPACE_BEGIN
class OpenGLTextureLoader
{
public:
	static std::shared_ptr<OpenGLTexture2D> CreateTexture2DFromFile(const std::string& filePath);
	static std::shared_ptr<OpenGLTexture2D> CreateTexture2DFromMemory(int width, int height, unsigned char* data);
};
TRV2_NAMESPACE_END