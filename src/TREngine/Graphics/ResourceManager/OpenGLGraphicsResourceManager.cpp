#pragma once
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLGraphicsResourceManager.h"
#include <Graphics/OpenGLProvider.h>
#include <Core/Utils/Utils.h>
#include <Core/Render/RawShader.h>
#include <Core/Render/Texture2D.h>
#include <Core/Render/ShaderProgram.h>

TRV2_NAMESPACE_BEGIN

static constexpr int MAX_BUFFSIZE = 1024;
static char infoLog[MAX_BUFFSIZE];
OpenGLGraphicsResourceManager::OpenGLGraphicsResourceManager(const EngineSettings& clientConfig)
{
	
}

OpenGLGraphicsResourceManager::~OpenGLGraphicsResourceManager()
{
}

ITextureHandle OpenGLGraphicsResourceManager::CreateTexture2D(glm::ivec2 size,
    const TextureParameters& parameters, PixelFormat srcFormat, EngineDataType dataType, const void* data)
{
    ITextureHandle handle;
    glGenTextures(1, &handle);
    ChangeTexture2D(handle, size, parameters, srcFormat, dataType, data);
    return handle;
}

void OpenGLGraphicsResourceManager::DeleteTexture2D(ITextureHandle handle)
{
    glDeleteTextures(1, &handle);
}

void OpenGLGraphicsResourceManager::ChangeTexture2D(ITextureHandle handle, glm::ivec2 size, const TextureParameters& parameters,
    PixelFormat srcFormat, EngineDataType dataType, const void* data)
{
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexImage2D(GL_TEXTURE_2D, 0, OpenGLProvider::MapPixelFormat(parameters.InternalFormat), size.x, size.y,
        0, OpenGLProvider::MapPixelFormat(srcFormat),
    OpenGLProvider::MapDataType(dataType), data);

    auto sampleMethod = OpenGLProvider::MapTextureSampleMethod(parameters.SampleMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampleMethod[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampleMethod[1]);
    if (parameters.SampleMethod == TextureSampleMethod::BI_LINEAR_MIPMAP)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    auto warpMethod = OpenGLProvider::MapTextureWarpMethod(parameters.WarpMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpMethod);

    glBindTexture(GL_TEXTURE_2D, 0);
}


IShaderHandle OpenGLGraphicsResourceManager::CreateRawShader(const char* code, ShaderType shaderType, const char* fileName)
{
    IShaderHandle id = glCreateShader(OpenGLProvider::MapShaderType(shaderType));
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
    return id;
}

void OpenGLGraphicsResourceManager::DeleteRawShader(IShaderHandle handle)
{
    glDeleteShader(handle);
}

IShaderProgramHandle OpenGLGraphicsResourceManager::CreateShaderProgram(const std::vector<const RawShader*>& shaders)
{
    IShaderProgramHandle id = glCreateProgram();
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
    return id;
}

void OpenGLGraphicsResourceManager::DeleteShaderProgram(IShaderProgramHandle handle)
{
    glDeleteProgram(handle);
}

IRenderTarget2DHandle OpenGLGraphicsResourceManager::CreateRenderTarget2D(Texture2D* receiver, glm::ivec2 size)
{
    IRenderTarget2DHandle handle;
    glGenFramebuffers(1, &handle);

    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, receiver->GetHandle(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::exception("Framebuffer not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return handle;
}

void OpenGLGraphicsResourceManager::DeleteRenderTarget2D(IRenderTarget2DHandle handle)
{
    glDeleteFramebuffers(1, &handle);
}

void OpenGLGraphicsResourceManager::SetShaderParameterMat4x4(ShaderProgram* shader, const std::string& name, const glm::mat4& mat, bool normalized)
{
    glUniformMatrix4fv(glGetUniformLocation(shader->GetHandle(), name.c_str()), 1, normalized, glm::value_ptr(mat));
}

void OpenGLGraphicsResourceManager::SetShaderParameter1i(ShaderProgram* shader, const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(shader->GetHandle(), name.c_str()), value);
}
TRV2_NAMESPACE_END