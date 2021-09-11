#include "RawShader.h"

#include <Core/Utils/Utils.h>
#include <array>

TRV2_NAMESPACE_BEGIN

RawShader::RawShader(IGraphicsResourceManager* resourceManager, const char* code, ShaderType shaderType, const char* fileName)
    : IGraphicsResource(resourceManager), _type(shaderType), _fileName(fileName)
{
    _handle = resourceManager->CreateRawShader(code, shaderType, fileName);
}

RawShader::~RawShader()
{
    _resourceManager->DeleteRawShader(_handle);
}
TRV2_NAMESPACE_END