#include "OpenGLTextureLoader.h"
#include <stbi/stb_image.h>

TRV2_NAMESPACE_BEGIN
std::shared_ptr<OpenGLTexture2D> trv2::OpenGLTextureLoader::LoadOpenGLShader(const std::string& filePath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::exception(stbi_failure_reason());
    }
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(data);
    return nullptr;
}
TRV2_NAMESPACE_END