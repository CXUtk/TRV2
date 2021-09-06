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
//
//template<>
//inline void OpenGLShader::SetParameter(const GLchar* name, int value) {
//    glUniform1i(glGetUniformLocation(this->_id, name), value);
//}
//
//template<>
//inline void OpenGLShader::SetParameter(const GLchar* name, float value) {
//    glUniform1f(glGetUniformLocation(this->_id, name), value);
//}
//
//template<>
//inline void OpenGLShader::SetParameter(const GLchar* name, glm::vec2 value) {
//    glUniform2fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));
//}
//
//template<>
//inline void OpenGLShader::SetParameter(const GLchar* name, glm::vec3 value) {
//    glUniform3fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));
//}
//
//template<>
//inline void OpenGLShader::SetParameter(const GLchar* name, glm::vec4 value) {
//    glUniform4fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));
//}
//
//template<>
//inline void OpenGLShader::SetParameter(const GLchar* name, glm::mat4 value) {
//    glUniformMatrix4fv(glGetUniformLocation(this->_id, name), 1, GL_FALSE, glm::value_ptr(value));
//}
//
//template<>
//inline void OpenGLShader::SetParameter(const GLchar* name, bool value) {
//    glUniform1i(glGetUniformLocation(this->_id, name), value);
//}
//
//template<typename T>
//inline void OpenGLShader::SetParameter(const GLchar* name, T value)
//{
//}
