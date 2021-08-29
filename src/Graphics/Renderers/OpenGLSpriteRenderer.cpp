#include "OpenGLSpriteRenderer.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Vertex2D
{
	glm::vec2 Position;
	glm::vec2 TextureCoords;

	Vertex2D(glm::vec2 pos, glm::vec2 texCoords) : Position(pos), TextureCoords(texCoords) {}
};

static const Vertex2D simpleQuadVertices[4] = {
	Vertex2D(glm::vec2(0, 0), glm::vec2(0, 0)),
	Vertex2D(glm::vec2(1, 0), glm::vec2(1, 0)),
	Vertex2D(glm::vec2(0, 1), glm::vec2(0, 1)),
	Vertex2D(glm::vec2(1, 1), glm::vec2(1, 1))
};

static const GLuint simpleQuadIndicies[6] = { 0, 1, 3, 0, 3, 2 };

OpenGLSpriteRenderer::OpenGLSpriteRenderer(const ITRGraphicsDevice* graphicsDevice, OpenGLShader* spriteShader)
	: _spriteShader(spriteShader)
{
	glGenVertexArrays(1, &_mainVAO);
	glGenBuffers(1, &_mainVBO);
	glGenBuffers(1, &_mainEBO);

	glBindVertexArray(_mainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _mainVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(simpleQuadVertices), simpleQuadVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mainEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(simpleQuadIndicies), simpleQuadIndicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)sizeof(glm::vec2));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

OpenGLSpriteRenderer::~OpenGLSpriteRenderer()
{
}

void OpenGLSpriteRenderer::Draw()
{
	_spriteShader->Apply();
	_spriteShader->SetParameter<glm::vec4>("uColor", glm::vec4(1, 0, 0, 1));
	_spriteShader->SetParameter<glm::mat4>("uTransform", glm::identity<glm::mat4>());
	{
		glBindVertexArray(_mainVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
