#pragma once
#include <TREngine_Interfaces.h>
#include <Assets/Assets_Interfaces.h>
#include <string>

TRV2_NAMESPACE_BEGIN
class OpenGLRawShader : public IRawShader
{
public:
	OpenGLRawShader(const char* code, ShaderType shaderType, const char* fileName);
	virtual ~OpenGLRawShader() override;

	virtual IShaderHandle GetHandle() const override { return _handle; }
	virtual ShaderType GetShaderType() const override { return _type; }
	virtual const char* GetFileName() const override { return _fileName.c_str(); }

private:	
	IShaderHandle _handle;
	ShaderType _type;
	std::string _fileName;
};
TRV2_NAMESPACE_END