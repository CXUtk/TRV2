#pragma once
#include <TREngine_Interfaces.h>
#include <Graphics/Graphics_Interfaces.h>
#include <vector>

TRV2_NAMESPACE_BEGIN

enum class ShaderType
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER,
	TESSELATION_SHADER,
	COMPUTE_SHADER,

	__COUNT,
};

class IRawShader
{
public:
	IRawShader(const char* code, ShaderType shaderType, const char* fileName) {}
	virtual ~IRawShader() = 0 {}

	virtual IShaderHandle GetHandle() const = 0;
	virtual ShaderType GetShaderType() const = 0;
	virtual const char* GetFileName() const = 0;
};
TRV2_NAMESPACE_END