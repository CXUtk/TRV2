#pragma once
#include <TRV2.h>

class OpenGLSpriteRenderer
{
public:
	OpenGLSpriteRenderer(const ITRGraphicsDevice * graphicsDevice);

	~OpenGLSpriteRenderer();


private:
	GLuint _mainVAO, _mainVBO, _mainEBO;
};