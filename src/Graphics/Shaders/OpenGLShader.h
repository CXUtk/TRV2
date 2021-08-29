#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class OpenGLShader
{
public:
    OpenGLShader(GLuint programID) : _id(programID) {}

    void Apply() { glUseProgram(_id); }

    template<typename T>
    void SetParameter(const GLchar* name, T value);

private:
    GLuint _id;
};

template<>
inline void OpenGLShader::SetParameter(const GLchar* name, int value) {
    glUniform1i(glGetUniformLocation(this->_id, name), value);
}

template<>
inline void OpenGLShader::SetParameter(const GLchar* name, float value) {
    glUniform1f(glGetUniformLocation(this->_id, name), value);
}

template<>
inline void OpenGLShader::SetParameter(const GLchar* name, glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));
}

template<>
inline void OpenGLShader::SetParameter(const GLchar* name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));
}

template<>
inline void OpenGLShader::SetParameter(const GLchar* name, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));
}

template<>
inline void OpenGLShader::SetParameter(const GLchar* name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(this->_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

template<>
inline void OpenGLShader::SetParameter(const GLchar* name, bool value) {
    glUniform1i(glGetUniformLocation(this->_id, name), value);
}

template<typename T>
inline void OpenGLShader::SetParameter(const GLchar* name, T value)
{
}
