#pragma once
#include <Core.h>
#include <string>
#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class RawShader : public IGraphicsResource
{
public:
	RawShader(IGraphicsResourceManager* resourceManager, const char* code, ShaderType shaderType, const char* fileName);
	virtual ~RawShader();

	virtual IShaderHandle GetHandle() const { return _handle; }
	virtual ShaderType GetShaderType() const { return _type; }
	virtual const char* GetFileName() const { return _fileName.c_str(); }

private:	
	IShaderHandle _handle;
	ShaderType _type;
	std::string _fileName;
};
TRV2_NAMESPACE_END