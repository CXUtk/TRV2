#pragma once

#include <TREngine_Interfaces.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Graphics/Interfaces/ITexture2D.h>

TRV2_NAMESPACE_BEGIN
class OpenGLTexture2D : public ITexture2D
{
public:
	OpenGLTexture2D(GLuint id, int width, int height);
	~OpenGLTexture2D();

	void Bind(int slot) const;
	unsigned int GetId() const override { return _id; }

	int GetWidth() const override { return _width; }
	int GetHeight() const override { return _height; }

private:
	GLuint _id;
	int _width, _height;
};
TRV2_NAMESPACE_END