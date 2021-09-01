#pragma once

#include <TREngine_Interfaces.h>
#include <Graphics/Interfaces/IShader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLShader : public IShader
{
public:
    OpenGLShader(GLuint programID) : _id(programID) {}

    void Apply() override;
    void SetParameteri1(const std::string& name, int value) override;
    void SetParameterfv2(const std::string& name, glm::vec2 value) override;
    void SetParameterfv4(const std::string& name, const glm::vec4& value) override;
    void SetParameterfm4x4(const std::string& name, const glm::mat4& value) override;

private:
    GLuint _id;
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
