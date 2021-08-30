#pragma once
#include <TRV2.h>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Shaders/OpenGLShader.h>
#include <vector>
#include "BatchInfo.h"

class OpenGLSpriteRenderer
{
public:
	OpenGLSpriteRenderer(const ITRGraphicsDevice* graphicsDevice, OpenGLShader* spriteShader);
	~OpenGLSpriteRenderer();

	void Begin(const glm::mat4& transform);
	void End();

	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);

private:
	GLuint _mainVAO, _mainVBO, _mainEBO;
	OpenGLShader* _spriteShader;

	std::vector<BatchState> _batchStateStack;

	int _currentVertex;
	std::unique_ptr<Vertex2D[]> _vertices;
	std::unique_ptr<GLuint[]> _vertexIndices;

	glm::mat4 getCurrentTransform() const;
	void pushQuad(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);
	void flush();
};