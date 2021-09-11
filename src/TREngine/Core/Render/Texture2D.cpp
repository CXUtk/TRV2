#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include <array>
#include "Texture2D.h"
#include <Core/Utils/Utils.h>
#include <Graphics/Graphics_Interfaces.h>


TRV2_NAMESPACE_BEGIN

Texture2D::Texture2D(IGraphicsResourceManager* resourceManager, int width, int height, const void* data)
    : IGraphicsResource(resourceManager), _width(width), _height(height), _parameters()
{
    assert(_graphicsDevice != nullptr);
    _handle = resourceManager->CreateTexture2D(width, height, data,
        PixelFormat::RGB, PixelFormat::RGBA, EngineDataType::UNSIGNED_BYTE, _parameters);
}

Texture2D::Texture2D(IGraphicsResourceManager* resourceManager, const std::string& fileName) 
    : IGraphicsResource(resourceManager)
{
    assert(_graphicsDevice != nullptr);
    int nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &_width, &_height, &nrChannels, 0);
    if (!data)
    {
        throw std::exception(string_format("Cannot load texture %s: %s", fileName.c_str(), stbi_failure_reason()).c_str());
    }
    _handle = resourceManager->CreateTexture2D(_width, _height, data,
        PixelFormat::RGB, PixelFormat::RGBA, EngineDataType::UNSIGNED_BYTE, TextureParameters());
    stbi_image_free(data);
}

Texture2D::Texture2D(IGraphicsResourceManager* resourceManager, int width, int height, const void* data,
    PixelFormat internalFormat, PixelFormat srcFormat, EngineDataType dataType, const TextureParameters& parameters):
    IGraphicsResource(resourceManager), _width(width), _height(height), _parameters(parameters)
{
    _handle = resourceManager->CreateTexture2D(width, height, data, internalFormat, srcFormat, dataType, parameters);
}

Texture2D::~Texture2D()
{
    _resourceManager->DeleteTexture2D(_handle);
}
TRV2_NAMESPACE_END