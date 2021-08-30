#include "OpenGLSpriteRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>


static const Vertex2D simpleQuadVertices[4] = {
	Vertex2D(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec4(1)),
	Vertex2D(glm::vec2(1, 0), glm::vec2(1, 0), glm::vec4(1)),
	Vertex2D(glm::vec2(0, 1), glm::vec2(0, 1), glm::vec4(1)),
	Vertex2D(glm::vec2(1, 1), glm::vec2(1, 1), glm::vec4(1))
};

static const GLuint simpleQuadIndicies[6] = { 0, 1, 3, 0, 3, 2 };
static constexpr int MaxQuadsPerBatch = 4096;
static constexpr int MaxVerticesPerBatch = MaxQuadsPerBatch * 4;
static constexpr int MaxIndiciesPerBatch = MaxQuadsPerBatch * 6;


OpenGLSpriteRenderer::OpenGLSpriteRenderer(const ITRGraphicsDevice* graphicsDevice, OpenGLShader* spriteShader)
	: _spriteShader(spriteShader)
{
	glGenVertexArrays(1, &_mainVAO);
	glGenBuffers(1, &_mainVBO);
	glGenBuffers(1, &_mainEBO);

	glBindVertexArray(_mainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _mainVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * MaxVerticesPerBatch, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mainEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * MaxQuadsPerBatch * 6, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)sizeof(glm::vec2));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)(2 * sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// 预先生成顶点index列表
	_vertexIndices = std::make_unique<GLuint[]>(MaxIndiciesPerBatch);
	_vertices = std::make_unique<Vertex2D[]>(MaxVerticesPerBatch);
	int cur = 0;
	for (int i = 0; i < MaxVerticesPerBatch; i += 4) {
		for (int j = 0; j < 6; j++) {
			_vertexIndices[cur++] = i + simpleQuadIndicies[j];
		}
	}
}

OpenGLSpriteRenderer::~OpenGLSpriteRenderer()
{
}

void OpenGLSpriteRenderer::Begin(const glm::mat4& transform)
{
	_batchStateStack.push_back(BatchState(transform));
	_currentVertex = 0;
}

void OpenGLSpriteRenderer::End()
{
	if (_currentVertex) flush();
	_batchStateStack.pop_back();
}

void OpenGLSpriteRenderer::Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, const glm::vec4& color)
{
	pushQuad(pos, size, origin, 0.f, color);
}

glm::mat4 OpenGLSpriteRenderer::getCurrentTransform() const
{
	return _batchStateStack.back().WorldTransform;
}

void OpenGLSpriteRenderer::pushQuad(glm::vec2 tpos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color)
{
	glm::mat4 transform;
	if (rotation != 0.f) {
		glm::rotate(rotation, glm::vec3(0, 0, 1));
	}
	for (int i = 0; i < 4; i++) {
		auto pos = (simpleQuadVertices[i].Position + origin) * size;
		auto vpos = (rotation == 0.f) ? (glm::vec4(pos, 0, 1)) : (transform * glm::vec4(pos, 0, 1));
		_vertices[_currentVertex].Position.x = vpos.x + tpos.x;
		_vertices[_currentVertex].Position.y = vpos.y + tpos.y;
		_vertices[_currentVertex].TextureCoords = simpleQuadVertices[i].TextureCoords;
		_vertices[_currentVertex].Color = color;
		_currentVertex++;
	}
	if (_currentVertex >= MaxVerticesPerBatch) {
		flush();
	}
}

void OpenGLSpriteRenderer::flush()
{
	_spriteShader->Apply();
	_spriteShader->SetParameter<glm::mat4>("uWorldTransform", getCurrentTransform());
	{
		int sz = _currentVertex;
		assert(sz % 4 == 0);
		glBindVertexArray(_mainVAO);
		// 以最多 MaxVerticesPerBatch 个点为单位，分批绘制线段
		for (int i = 0; i < _currentVertex; i += MaxVerticesPerBatch) {

			// 顶点数据输送
			glBindBuffer(GL_ARRAY_BUFFER, _mainVBO);
			int count = std::min(sz - i, MaxVerticesPerBatch);
			assert(count % 4 == 0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex2D) * count, _vertices.get());

			// 顶点顺序数据
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mainEBO);
			int idxCount = count / 4 * 6;
			assert(idxCount % 6 == 0);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * idxCount, _vertexIndices.get());

			// 绘制
			glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}

	_currentVertex = 0;
}
