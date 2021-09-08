#pragma once

#include <TREngine_Interfaces.h>
#include <Graphics/Graphics_Interfaces.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <Core.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLShaderProgram
{
public:
    explicit OpenGLShaderProgram(const std::shared_ptr<IRawShader>& vertexShader, const std::shared_ptr<IRawShader>& fragmentShader);
    explicit OpenGLShaderProgram(const std::vector<std::shared_ptr<IRawShader>>& shaders);

    virtual ~OpenGLShaderProgram();

    virtual IShaderProgramHandle GetHandle() const { return _handle; }

    void Apply();
    void SetParameteri1(const std::string& name, int value);
    void SetParameterfv2(const std::string& name, glm::vec2 value);
    void SetParameterfv4(const std::string& name, const glm::vec4& value);
    void SetParameterfm4x4(const std::string& name, const glm::mat4& value);
    void SetParameterfvArray(const std::string& name, const float* data, int size);
    void SetParameterintvArray(const std::string& name, const int* data, int size);
private:
    IShaderProgramHandle _handle;
};
TRV2_NAMESPACE_END