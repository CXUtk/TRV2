#pragma once
#include <TRV2.h>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Shaders/OpenGLShader.h>

class OpenGLSpriteRenderer
{
public:
	OpenGLSpriteRenderer(const ITRGraphicsDevice * graphicsDevice, OpenGLShader* spriteShader);
	~OpenGLSpriteRenderer();

	void Draw();

private:
	GLuint _mainVAO, _mainVBO, _mainEBO;
	OpenGLShader* _spriteShader;
};