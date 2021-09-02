#pragma once

#include <TREngine_Interfaces.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLTexture2D
{
public:
	OpenGLTexture2D(GLuint id);
	~OpenGLTexture2D();

	void Bind(int slot) const;
	GLuint GetID() const { return _id; }

private:
	GLuint _id;
};
TRV2_NAMESPACE_END