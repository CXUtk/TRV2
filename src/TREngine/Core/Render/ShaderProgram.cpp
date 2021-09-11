#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#include <Core/Render/RawShader.h>
#include <Core/Utils/Utils.h>

TRV2_NAMESPACE_BEGIN
static const int MAX_BUFFSIZE = 1024;
static char infoLog[MAX_BUFFSIZE];

ShaderProgram::ShaderProgram(IGraphicsResourceManager* resourceManager, 
    const RawShader* vertexShader, const RawShader* fragmentShader)
    : IGraphicsResource(resourceManager)
{
	_handle = resourceManager->CreateShaderProgram({ vertexShader, fragmentShader });
}
ShaderProgram::ShaderProgram(IGraphicsResourceManager* resourceManager, const std::vector<const RawShader*>& shaders)
    : IGraphicsResource(resourceManager)
{
    _handle = resourceManager->CreateShaderProgram(shaders);
}
ShaderProgram::~ShaderProgram()
{
    _resourceManager->DeleteShaderProgram(_handle);
}

void ShaderProgram::SetParameterfm4x4(const std::string& name, const glm::mat4& value)
{
    _resourceManager->SetShaderParameterMat4x4(this, name, value, false);
}
void ShaderProgram::SetParameter1i(const std::string& name, int value)
{
    _resourceManager->SetShaderParameter1i(this, name, value);
}
TRV2_NAMESPACE_END