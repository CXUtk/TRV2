#pragma once
#include <vector>
#include <memory>
#include <map>

#include <TREngine_Interfaces.h>
#include <Graphics/Graphics_Interfaces.h>
#include <Graphics/Structures/BatchInfo.h>

TRV2_NAMESPACE_BEGIN
class OpenGLSpriteRenderer : public ISpriteRenderer
{
public:
	explicit OpenGLSpriteRenderer(const IGraphicsDevice* graphicsDevice);
	~OpenGLSpriteRenderer() override;

	void Begin(const glm::mat4& transform) override;
	void End() override;

	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color) override;
	void Draw(const ITexture2D* texture, glm::vec2 pos, glm::vec2 size,
		glm::vec2 origin, float rotation, const glm::vec4& color) override;

private:
	// OpenGL绘制用的
	IGraphicsHandle _quadVAO;
	IGraphicsHandle _quadBuffers[2];

	std::vector<BatchState> _batchStateStack;
	const IGraphicsDevice* _graphicsDevice;

	// 缓存，顶点，顶点序号等
	int _currentVertex;
	std::unique_ptr<BatchVertex2D[]> _vertices;
	std::unique_ptr<unsigned int[]> _vertexIndices;

	int _currentTextureSlots;
	std::unique_ptr<IGraphicsHandle[]> _usedTextures;


	// 用到的Shader
	std::shared_ptr<IShader> _spriteShaderPure;

	// 用到的Texture
	std::shared_ptr<ITexture2D> _whiteTexture;

	
	glm::mat4 getCurrentTransform() const;

	void pushTextureQuad(const ITexture2D* texture, glm::vec2 tpos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);

	void flushBatch();
	
	int findUsedTexture(const ITexture2D* texture) const;
	void bindTextures() const;
};
TRV2_NAMESPACE_END