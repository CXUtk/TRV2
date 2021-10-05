#pragma once
#include <vector>
#include <memory>
#include <map>

#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>
#include <Core/Structures/VertexLayout.h>
#include <Core/Structures/Rect.hpp>

TRV2_NAMESPACE_BEGIN

struct BatchInfo
{
	glm::vec2 Position, Size;
	glm::vec4 Color;
	BatchInfo() = default;
	BatchInfo(glm::vec2 pos, glm::vec2 size, const glm::vec4& color) : Position(pos), Size(size), Color(color) {}
};

enum class SpriteSortMode
{
	Deferred,
	FrontToBack,
	BackToFront,
	Texture,
};

enum class SpriteFlipMode
{
	None,
	FlipHorizontal,
	FlipVertical
};

struct BatchSettings
{
	SpriteSortMode SpriteSortMode = SpriteSortMode::Deferred;
	BlendingMode BlendMode = BlendingMode::None;
	ShaderProgram* Shader;

	BatchSettings() : Shader(nullptr) {}
};

struct BatchState
{
	glm::mat4 WorldTransform{};
	bool IsBatchBegin = false;
	BatchSettings Settings{};
	BatchState() = default;
};

struct BYTE_Color
{
	byte8 R;
	byte8 G;
	byte8 B;
	byte8 A;
};

inline BYTE_Color vec4ToByteColor(const glm::vec4& color)
{
	BYTE_Color C;
	C.R = (unsigned char)(std::min(OneMinusEpsilon, color.r) * 256);
	C.G = (unsigned char)(std::min(OneMinusEpsilon, color.g) * 256);
	C.B = (unsigned char)(std::min(OneMinusEpsilon, color.b) * 256);
	C.A = (unsigned char)(std::min(OneMinusEpsilon, color.a) * 256);
	return C;
}

struct BatchVertex2D
{
	glm::vec2 Position;
	glm::vec2 TextureCoords;
	BYTE_Color Color;
	float TextureIndex;

	BatchVertex2D() = default;
	BatchVertex2D(glm::vec2 pos, glm::vec2 texCoords, const glm::vec4& color) : Position(pos),
		TextureCoords(texCoords), TextureIndex(0.f)
	{
		Color = vec4ToByteColor(color);
	}
};


class SpriteRenderer
{
public:
	explicit SpriteRenderer(IGraphicsDevice* graphicsDevice, ShaderProgram * spriteShader,
		Texture2D * pureTexture);
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
	void Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color,
		SpriteFlipMode flipMode = SpriteFlipMode::None);

	void Draw(glm::vec2 pos, glm::vec2 size, const trv2::Rectf& texCoords,
		glm::vec2 origin, float rotation, const glm::vec4& color,
		SpriteFlipMode flipMode = SpriteFlipMode::None);

	/**
	 * @brief Push a sprite drawing task to the batch list
	 * @param texture The texture to draw
	 * @param pos Bottom-right starting point of this quad
	 * @param size Size of the quad
	 * @param origin The transformation center of this quad
	 * @param rotation Rotation of this quad
	 * @param color Color of this sprite (multiply with original texture)
	*/
	void Draw(const Texture2D* texture, glm::vec2 pos, glm::vec2 size,
		glm::vec2 origin, float rotation, const glm::vec4& color,
		SpriteFlipMode flipMode = SpriteFlipMode::None);

	void Draw(const Texture2D* texture, glm::vec2 pos, glm::vec2 size, const trv2::Rectf& texCoords,
		glm::vec2 origin, float rotation, const glm::vec4& color,
		SpriteFlipMode flipMode = SpriteFlipMode::None);

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
	std::unique_ptr<const Texture2D*[]> _usedTextures;

	// 用到的Shader
	ShaderProgram* _spriteShaderPure = nullptr;

	// 用到的Texture
	Texture2D* _whiteTexture = nullptr;


	void pushTextureQuad(const Texture2D* texture, glm::vec2 tpos, glm::vec2 size, 
		glm::vec2 origin, float rotation, const glm::vec4& color, SpriteFlipMode flipMode,const glm::vec2* useTexCoords);

	void flushBatch();
	int findUsedTexture(const Texture2D* texture) const;
	void bindTextures() const;
};
TRV2_NAMESPACE_END