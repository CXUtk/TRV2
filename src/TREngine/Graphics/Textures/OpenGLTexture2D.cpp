#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include <TREngine_Interfaces.hpp>
#include "OpenGLTexture2D.hpp"
#include <Utils/Utils.hpp>
#include <array>


TRV2_NAMESPACE_BEGIN

OpenGLTexture2D::OpenGLTexture2D(IGraphicsDevice* device, int width, int height, unsigned char* data) :
    OpenGLTexture2D(device, width, height, data, PixelFormat::RGB, PixelFormat::RGBA, EngineDataType::UNSIGNED_BYTE, TextureParameters())
{
}

OpenGLTexture2D::OpenGLTexture2D(IGraphicsDevice* device, const std::string& fileName)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        throw std::exception(string_format("Cannot load texture %s: %s", fileName.c_str(), stbi_failure_reason()).c_str());
    }
    genNewTexture2D(device, width, height, data, PixelFormat::RGB, PixelFormat::RGBA, EngineDataType::UNSIGNED_BYTE, TextureParameters());
    stbi_image_free(data);
}

OpenGLTexture2D::OpenGLTexture2D(IGraphicsDevice* device, int width, int height, unsigned char* data,
    PixelFormat internalFormat, PixelFormat srcFormat, EngineDataType dataType, const TextureParameters& parameters)
{
    genNewTexture2D(device, width, height, data, internalFormat, srcFormat, dataType, parameters);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &_handle);
}

void OpenGLTexture2D::genNewTexture2D(IGraphicsDevice* device, int width, int height, unsigned char* data,
    PixelFormat internalFormat, PixelFormat srcFormat, EngineDataType dataType, const TextureParameters& parameters)
{
    assert(_graphicsDevice != nullptr);
    assert(data != nullptr);

    _graphicsDevice = device;
    _width = width;
    _height = height;
    _parameters = parameters;

    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_2D, _handle);

    glTexImage2D(GL_TEXTURE_2D, 0, _OpenGLAPI::MapPixelFormat(internalFormat), width, height, 0, _OpenGLAPI::MapPixelFormat(srcFormat), 
        _OpenGLAPI::MapDataType(EngineDataType::UNSIGNED_BYTE), data);

    auto sampleMethod = _OpenGLAPI::MapTextureSampleMethod(parameters.SampleMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampleMethod[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampleMethod[1]);

    auto warpMethod = _OpenGLAPI::MapTextureWarpMethod(parameters.TextureWarpMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpMethod);
}
TRV2_NAMESPACE_END