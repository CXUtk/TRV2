#pragma once
#include <glm/glm.hpp>
#include <vector>

#include <Graphics/Graphics_Interfaces.h>
#include <Core.h>

TRV2_NAMESPACE_BEGIN

enum class ShaderUniformType
{
    Int,
    Int2,
    Int3,
    Int4,
    Float,
    Float2,
    Float3,
    Float4,
    FloatVector,
    Matrix4x4,
};

class ShaderProgram : public IGraphicsResource
{
public:
    ShaderProgram(IGraphicsResourceManager* resourceManager, const RawShader* vertexShader, const RawShader* fragmentShader);
    ShaderProgram(IGraphicsResourceManager* resourceManager, const std::vector<const RawShader*>& shaders);

    virtual ~ShaderProgram();

    virtual IShaderProgramHandle GetHandle() const { return _handle; }

    void SetParameterfm4x4(const std::string& name, const glm::mat4& value);
    void SetParameter1i(const std::string& name, int value);
private:
    IShaderProgramHandle _handle;
};
TRV2_NAMESPACE_END