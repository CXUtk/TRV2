#include "OpenGLShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <Assets/Assets_Interfaces.h>
#include <Utils/Utils.h>

TRV2_NAMESPACE_BEGIN
static const int MAX_BUFFSIZE = 1024;
static char infoLog[MAX_BUFFSIZE];

OpenGLShaderProgram::OpenGLShaderProgram(std::shared_ptr<IRawShader> vertexShader, std::shared_ptr<IRawShader> fragmentShader) : IShaderProgram(vertexShader, fragmentShader)
{
    GLuint id = glCreateProgram();
    glAttachShader(id, vertexShader->GetHandle());
    glAttachShader(id, fragmentShader->GetHandle());
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
        throw std::exception(string_format("Failed to link shader with %s and %s: %s", vertexShader->GetFileName(), fragmentShader->GetFileName(), infoLog).c_str());
    }
    _handle = id;
}
OpenGLShaderProgram::OpenGLShaderProgram(const std::vector<std::shared_ptr<IRawShader>>& shaders) : IShaderProgram(shaders)
{
    GLuint id = glCreateProgram();
    for (auto& shader : shaders)
    {
        glAttachShader(id, shader->GetHandle());
    }
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
        std::string str = "[";
        for (auto& shader : shaders)
        {
            str.append(shader->GetFileName());
            str.push_back(',');
        }
        str.push_back(']');
        throw std::exception(string_format("Failed to link shader with %s: %s", str.c_str(), infoLog).c_str());
    }
    _handle = id;
}
OpenGLShaderProgram::~OpenGLShaderProgram()
{
	glDeleteProgram(_handle);
}
void OpenGLShaderProgram::Apply() {
	glUseProgram(_handle);
}

void OpenGLShaderProgram::SetParameteri1(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(this->_handle, name.c_str()), value);
}

void OpenGLShaderProgram::SetParameterfv2(const std::string& name, glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(this->_handle, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShaderProgram::SetParameterfv4(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(this->_handle, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShaderProgram::SetParameterfm4x4(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->_handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void OpenGLShaderProgram::SetParameterfvArray(const std::string& name, const float* data, int size)
{
	glUniform1fv(glGetUniformLocation(this->_handle, name.c_str()), size, data);
}
void OpenGLShaderProgram::SetParameterintvArray(const std::string& name, const int* data, int size)
{
	glUniform1iv(glGetUniformLocation(this->_handle, name.c_str()), size, data);
}
TRV2_NAMESPACE_END