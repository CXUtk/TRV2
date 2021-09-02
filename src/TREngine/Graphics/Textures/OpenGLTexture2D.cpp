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

void OpenGLTexture2D::Bind(int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _id);
}

TRV2_NAMESPACE_END