#pragma once

#include <TREngine_Interfaces.h>
#include "OpenGLTexture2D.h"

TRV2_NAMESPACE_BEGIN
OpenGLTexture2D::OpenGLTexture2D(GLuint id, int width, int height) : _id(id), _width(width), _height(height)
{
}

OpenGLTexture2D::~OpenGLTexture2D()
{
}
TRV2_NAMESPACE_END