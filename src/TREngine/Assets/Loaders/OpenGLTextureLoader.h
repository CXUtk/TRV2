#pragma once
#include <memory>
#include <string>
#include <TREngine_Interfaces.h>
#include <Graphics/Textures/OpenGLTexture2D.h>

TRV2_NAMESPACE_BEGIN
class OpenGLTextureLoader
{
public:
	static std::shared_ptr<OpenGLTexture2D> LoadOpenGLShader(const std::string& filePath);
};
TRV2_NAMESPACE_END