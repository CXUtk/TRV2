#pragma once
#include <memory>
#include <string>
#include <TREngine_Interfaces.h>
#include <Graphics/Shaders/OpenGLShader.h>

TRV2_NAMESPACE_BEGIN
class OpenGLShaderLoader
{
public:
	static std::shared_ptr<OpenGLShader> LoadOpenGLShader(const std::string& vertex, const std::string& fragment);
};
TRV2_NAMESPACE_END