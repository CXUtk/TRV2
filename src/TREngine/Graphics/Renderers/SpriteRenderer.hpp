#pragma once
#include <vector>
#include <memory>
#include <map>

#include <TREngine_Interfaces.hpp>
#include <Core.hpp>

#include <Graphics/Graphics_Interfaces.hpp>
#include <Graphics/Structures/BatchInfo.hpp>
#include <Graphics/Structures/VertexLayout.hpp>

TRV2_NAMESPACE_BEGIN
class SpriteRenderer
{
public:
	explicit SpriteRenderer(IGraphicsDevice* graphicsDevice, IShaderProgram * spriteShader,
		ITexture2D * pureTexture);
	~SpriteRenderer();

	/**
	 * @brief Begin a drawing batch
	 * @param transform The transform matrix of this drawing batch
	 * @param settings The settings of this drawing batch
	*/
	void Begin(const glm::mat4& transform, const BatchSettings& settings);

	/**
	 * @brief End current drawing batch and flush the batch to graphics device
	*/
	void End();

	/**
	 * @brief Push a sprite drawing task to batch list. This drawing task will use default texture.
	 * @param pos Bottom-right starting point of this quad
	 * @param size Size of this quad
	 * @param origin The transformation center of this quad
	 * @param rotation Rotation of this quad
	 * @param color Color of this sprite (multiply with original texture)
	*/
	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color);

	/**
	 * @brief Push a sprite drawing task to batch list
	 * @param texture The texture to draw
	 * @param pos Bottom-right starting point of this quad
	 * @param size Size of the quad
	 * @param origin The transformation center of this quad
	 * @param rotation Rotation of this quad
	 * @param color Color of this sprite (multiply with original texture)
	*/
	void Draw(const ITexture2D* texture, glm::vec2 pos, glm::vec2 size,
		glm::vec2 origin, float rotation, const glm::vec4& color);

private:
	// 绘制用的
	IVertexBufferHandle _quadVAO;
	IVertexBufferHandle _quadBuffers[2];

	BatchState _batchState;
	IGraphicsDevice* _graphicsDevice;

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