#include "OpenGLShaderLoader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Utils/Utils.h>
#include <Graphics/Shaders/OpenGLShader.h>


static const int MAX_BUFFSIZE = 1024;
static char infoLog[MAX_BUFFSIZE];

static GLuint compileShader(const std::string& shaderCode, GLenum shaderType, const std::string& shaderFile) {
    GLuint id = glCreateShader(shaderType);
    const char* code = shaderCode.c_str();
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
        throw std::exception(string_format("Failed to compile shader %s: %s", shaderFile.c_str(), infoLog).c_str());
    }
    return id;
}


std::shared_ptr<OpenGLShader> OpenGLShaderLoader::LoadOpenGLShader(const std::string& vertexFile, const std::string& fragmentFile)
{
	auto vertexShader = ReadAllStringFromFile(vertexFile), fragmentShader = ReadAllStringFromFile(fragmentFile);
	auto vertex = compileShader(vertexShader, GL_VERTEX_SHADER, vertexFile);
	auto fragment = compileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentFile);
    auto id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, MAX_BUFFSIZE, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
        throw std::exception(string_format("Failed to link shader %s: %s", vertexFile.c_str(), infoLog).c_str());
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return std::make_shared<OpenGLShader>(id);
}
