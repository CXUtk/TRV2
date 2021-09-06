#pragma once
#include <vector>
#include <memory>
#include <map>

#include <TREngine_Interfaces.h>
#include <Graphics/Graphics_Interfaces.h>
#include <Graphics/Structures/BatchInfo.h>
#include <Graphics/Structures/VertexLayout.h>

TRV2_NAMESPACE_BEGIN
class SpriteRenderer
{
public:
	explicit SpriteRenderer(const IGraphicsDevice* graphicsDevice, IShaderProgram * spriteShader,
		ITexture2D * pureTexture);
	~SpriteRenderer();

	void Begin(const glm::mat4& transform, const BatchSettings& settings);
	void End();

	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);
	void Draw(const ITexture2D* texture, glm::vec2 pos, glm::vec2 size,
		glm::vec2 origin, float rotation, const glm::vec4& color);

private:
	// OpenGL绘制用的
	IVertexBufferHandle _quadVAO;
	IVertexBufferHandle _quadBuffers[2];

	BatchState _batchState;
	const IGraphicsDevice* _graphicsDevice;

	// 缓存，顶点，顶点序号等
	int _currentVertex;
	std::unique_ptr<BatchVertex2D[]> _vertices;
	std::unique_ptr<unsigned int[]> _vertexIndices;

	int _currentTextureSlots;
	std::unique_ptr<const ITexture2D*[]> _usedTextures;


	// 用到的Shader
	IShaderProgram* _spriteShaderPure;

	// 用到的Texture
	ITexture2D* _whiteTexture;


	void pushTextureQuad(const ITexture2D* texture, glm::vec2 tpos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);
	void flushBatch();
	int findUsedTexture(const ITexture2D* texture) const;
	void bindTextures() const;
};
TRV2_NAMESPACE_END