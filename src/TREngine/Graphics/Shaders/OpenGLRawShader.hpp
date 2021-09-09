#pragma once
#include <TREngine/TREngine_Interfaces.hpp>
#include <string>
#include <Core.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLRawShader
{
public:
	OpenGLRawShader(const char* code, ShaderType shaderType, const char* fileName);
	virtual ~OpenGLRawShader();

	virtual IShaderHandle GetHandle() const { return _handle; }
	virtual ShaderType GetShaderType() const { return _type; }
	virtual const char* GetFileName() const { return _fileName.c_str(); }

private:	
	IShaderHandle _handle;
	ShaderType _type;
	std::string _fileName;
};
TRV2_NAMESPACE_END