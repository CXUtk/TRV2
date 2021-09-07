#pragma once

#include <TREngine_Interfaces.h>
#include <Graphics/Graphics_Interfaces.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

TRV2_NAMESPACE_BEGIN
class OpenGLShaderProgram : public IShaderProgram
{
public:
    explicit OpenGLShaderProgram(const std::shared_ptr<IRawShader>& vertexShader, const std::shared_ptr<IRawShader>& fragmentShader);
    explicit OpenGLShaderProgram(const std::vector<std::shared_ptr<IRawShader>>& shaders);

    virtual ~OpenGLShaderProgram() override;

    virtual IShaderProgramHandle GetHandle() const override { return _handle; }

    void Apply() override;
    void SetParameteri1(const std::string& name, int value) override;
    void SetParameterfv2(const std::string& name, glm::vec2 value) override;
    void SetParameterfv4(const std::string& name, const glm::vec4& value) override;
    void SetParameterfm4x4(const std::string& name, const glm::mat4& value) override;
    void SetParameterfvArray(const std::string& name, const float* data, int size) override;
    void SetParameterintvArray(const std::string& name, const int* data, int size) override;
private:
    IShaderProgramHandle _handle;
};
TRV2_NAMESPACE_END