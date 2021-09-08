#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include <TREngine_Interfaces.hpp>
#include "OpenGLTexture2D.hpp"
#include <Utils/Utils.hpp>


TRV2_NAMESPACE_BEGIN
OpenGLTexture2D::OpenGLTexture2D(IGraphicsDevice* device, ITextureHandle handle) : 
    _graphicsDevice(device), _handle(handle)
{
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_height);
}

OpenGLTexture2D::OpenGLTexture2D(IGraphicsDevice* device, int width, int height, unsigned char* data) :
    _graphicsDevice(device), _width(width), _height(height)
{
    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_2D, _handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenGLTexture2D::OpenGLTexture2D(IGraphicsDevice* device, const std::string& fileName) : _graphicsDevice(device)
{
    int nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &_width, &_height, &nrChannels, 0);
    if (!data)
    {
        throw std::exception(string_format("Cannot load texture %s: %s", fileName.c_str(), stbi_failure_reason()).c_str());
    }
    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_2D, _handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &_handle);
}
TRV2_NAMESPACE_END