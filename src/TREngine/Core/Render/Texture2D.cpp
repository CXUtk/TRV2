#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include <array>
#include "Texture2D.h"
#include <Core/Utils/Utils.h>
#include <Graphics/Graphics_Interfaces.h>


TRV2_NAMESPACE_BEGIN

Texture2D::Texture2D(IGraphicsResourceManager* resourceManager, glm::ivec2 size, const void* data)
    : IGraphicsResource(resourceManager), _size(size), _parameters()
{
    assert(resourceManager != nullptr);
    _handle = resourceManager->CreateTexture2D(size,
       _parameters, PixelFormat::RGBA, EngineDataType::UNSIGNED_BYTE, data);
}

Texture2D::Texture2D(IGraphicsResourceManager* resourceManager, const std::string& fileName)
    : IGraphicsResource(resourceManager)
{
    assert(resourceManager != nullptr);
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        throw std::exception(string_format("Cannot load texture %s: %s", fileName.c_str(), stbi_failure_reason()).c_str());
    }
    _size = glm::ivec2(width, height);
    TextureParameters parameters{};
    parameters.InternalFormat = PixelFormat::RGBA;

    PixelFormat targetFormat = PixelFormat::RGB;
    if (nrChannels == 4)
    {
        targetFormat = PixelFormat::RGBA;
    }
    _handle = resourceManager->CreateTexture2D(_size, parameters,
         targetFormat, EngineDataType::UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

Texture2D::Texture2D(IGraphicsResourceManager* resourceManager, glm::ivec2 size, const TextureParameters& parameters,
    PixelFormat srcFormat, EngineDataType dataType, const void* data)
    : IGraphicsResource(resourceManager), _size(size), _parameters(parameters)
{
    _handle = resourceManager->CreateTexture2D(size, parameters, srcFormat, dataType, data);
}

Texture2D::~Texture2D()
{
    _resourceManager->DeleteTexture2D(_handle);
}
void Texture2D::ChangeBuffer(glm::ivec2 size, const TextureParameters& parameters, PixelFormat srcFormat, EngineDataType dataType, const void* data)
{
    _size = size;
    _parameters = parameters;
    _resourceManager->ChangeTexture2D(_handle, size, _parameters, srcFormat, dataType, data);
}
TRV2_NAMESPACE_END