#pragma once
#include "BatchInfo.h"

#include <TREngine_Interfaces.h>
#include <Graphics/Shaders/OpenGLShader.h>
#include <Graphics/Interfaces/ISpriteRenderer.h>
#include <Graphics/Textures/OpenGLTexture2D.h>

#include <vector>
#include <memory>

TRV2_NAMESPACE_BEGIN
class OpenGLSpriteRenderer : public ISpriteRenderer
{
public:
	OpenGLSpriteRenderer(const ITRGraphicsDevice* graphicsDevice);
	~OpenGLSpriteRenderer();

	void Begin(const glm::mat4& transform) override;
	void End() override;

	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color) override;
	void Draw(const OpenGLTexture2D& texture, glm::vec2 pos, glm::vec2 size,
		glm::vec2 origin, float rotation, const glm::vec4& color);

private:
	// OpenGL绘制用的
	GLuint _mainVAO, _mainVBO, _mainEBO;
	std::vector<BatchState> _batchStateStack;

	// 缓存，顶点，顶点序号等
	int _currentVertex;
	std::unique_ptr<BatchVertex2D[]> _vertices;
	std::unique_ptr<GLuint[]> _vertexIndices;

	// 用到的Shader
	std::shared_ptr<OpenGLShader> _spriteShaderPure;

	// OpenGL接口相关，常量相关
	int _maxTextureUnits;

	glm::mat4 getCurrentTransform() const;
	void pushQuad(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);
	void flush();
};
TRV2_NAMESPACE_END