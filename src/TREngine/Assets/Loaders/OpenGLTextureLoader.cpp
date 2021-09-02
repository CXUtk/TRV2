#include "OpenGLTextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <Utils/Utils.h>

TRV2_NAMESPACE_BEGIN
std::shared_ptr<OpenGLTexture2D> trv2::OpenGLTextureLoader::CreateTexture2D(const std::string& filePath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::exception(string_format("Cannot load texture %s: %s", filePath.c_str(), stbi_failure_reason()).c_str());
    }
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(data);
    return std::make_shared<OpenGLTexture2D>(textureId, width, height);
}
std::shared_ptr<OpenGLTexture2D> OpenGLTextureLoader::CreateTexture2D(int width, int height, unsigned char* data)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return std::make_shared<OpenGLTexture2D>(textureId, width, height);
}
TRV2_NAMESPACE_END