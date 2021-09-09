#include "OpenGLRawShader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Utils/Utils.hpp>
#include <array>

TRV2_NAMESPACE_BEGIN
static constexpr int MAX_BUFFSIZE = 1024;
static char infoLog[MAX_BUFFSIZE];

trv2::OpenGLRawShader::OpenGLRawShader(const char* code, ShaderType shaderType, const char* fileName) : 
    _type(shaderType), _fileName(fileName)
{
    GLuint id = glCreateShader(_OpenGLAPI::MapShaderType(shaderType));
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
        // 需不需要 glDeleteShader(_handle) 呢?
        throw std::exception(string_format("Failed to compile shader %s: %s", fileName, infoLog).c_str());
    }
    _handle = id;
}


OpenGLRawShader::~OpenGLRawShader()
{
    glDeleteShader(_handle);
}
TRV2_NAMESPACE_END