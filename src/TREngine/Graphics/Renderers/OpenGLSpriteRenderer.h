#pragma once
#include "BatchInfo.h"

#include <TREngine_Interfaces.h>
#include <Graphics/Shaders/OpenGLShader.h>
#include <Graphics/Interfaces/ISpriteRenderer.h>
#include <Graphics/Textures/OpenGLTexture2D.h>

#include <vector>
#include <memory>
#include <map>

TRV2_NAMESPACE_BEGIN
class OpenGLSpriteRenderer : public ISpriteRenderer
{
public:
	OpenGLSpriteRenderer(const ITRGraphicsDevice* graphicsDevice);
	~OpenGLSpriteRenderer();

	void Begin(const glm::mat4& transform) override;
	void End() override;

	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color) override;
	void Draw(const ITexture2D& texture, glm::vec2 pos, glm::vec2 size,
		glm::vec2 origin, float rotation, const glm::vec4& color);

private:
	// OpenGL接口相关，常量相关
	int _maxTextureUnits;


	// OpenGL绘制用的
	GLuint _mainVAO, _mainVBO, _mainEBO;
	std::vector<BatchState> _batchStateStack;

	// 缓存，顶点，顶点序号等
	int _currentVertex;
	std::unique_ptr<BatchVertex2D[]> _vertices;
	std::unique_ptr<GLuint[]> _vertexIndices;
	std::map<GLuint, int> _usedTextures;
	std::vector<const ITexture2D*> _textureRefs;

	std::unique_ptr<float[]> _textureSlotsBuffer;

	// 用到的Shader
	std::shared_ptr<IShader> _spriteShaderPure;

	// 用到的Texture
	std::shared_ptr<ITexture2D> _whiteTexture;

	glm::mat4 getCurrentTransform() const;

	void pushTextureQuad(const ITexture2D& texture, glm::vec2 tpos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);

	void flushBatch();
};
TRV2_NAMESPACE_END