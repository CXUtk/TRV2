#pragma once
#include <memory>
#include <string>
#include <TRV2.h>

class OpenGLShaderLoader
{
public:
	static std::shared_ptr<OpenGLShader> LoadOpenGLShader(const std::string& vertex, const std::string& fragment);
};