#pragma once
#include <memory>
#include <string>
#include <TREngine_Interfaces.h>
#include <Graphics/Textures/OpenGLTexture.h>

TRV2_NAMESPACE_BEGIN
class OpenGLTextureLoader
{
public:
	static std::shared_ptr<OpenGLTexture> LoadOpenGLShader(const std::string& filePath);
};
TRV2_NAMESPACE_END