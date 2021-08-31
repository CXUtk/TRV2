#pragma once
#include "BatchInfo.h"

#include <Interfaces.h>
#include <Graphics/Shaders/OpenGLShader.h>
#include <Graphics/Interfaces/ISpriteRenderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>


class OpenGLSpriteRenderer : public ISpriteRenderer
{
public:
	OpenGLSpriteRenderer(const ITRGraphicsDevice* graphicsDevice, OpenGLShader* spriteShader);
	~OpenGLSpriteRenderer();

	void Begin(const glm::mat4& transform) override;
	void End() override;

	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color) override;

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