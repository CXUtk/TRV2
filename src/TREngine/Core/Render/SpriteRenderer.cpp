#include "SpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <algorithm>

#include <Core/Utils/Utils.h>

#include <Core/Render/ShaderProgram.h>
#include <Core/Render/Texture2D.h>

TRV2_NAMESPACE_BEGIN
static const BatchVertex2D simpleQuadVertices[4] = {
	BatchVertex2D(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec4(1)),
	BatchVertex2D(glm::vec2(1, 0), glm::vec2(1, 0), glm::vec4(1)),
	BatchVertex2D(glm::vec2(0, 1), glm::vec2(0, 1), glm::vec4(1)),
	BatchVertex2D(glm::vec2(1, 1), glm::vec2(1, 1), glm::vec4(1))
};

static const glm::vec2 defaultTexCoordsList[4] = {
	glm::vec2(0, 0),
	glm::vec2(1, 0),
	glm::vec2(0, 1),
	glm::vec2(1, 1)
};

static const uint defaultSpriteTexOrder[4] = {
	0, 1, 2, 3
};
static const uint flipHorizontalSpriteTexOrder[4] = {
	1, 0, 3, 2
};
static const uint flipVerticalSpriteTexOrder[4] = {
	2, 3, 0, 1
};

static constexpr unsigned int simpleQuadIndicies[6] = { 0, 1, 3, 0, 3, 2 };
static constexpr int MaxQuadsPerBatch = 1 << 16;
static constexpr int MaxVerticesPerBatch = MaxQuadsPerBatch * 4;
static constexpr int MaxIndiciesPerBatch = MaxQuadsPerBatch * 6;

SpriteRenderer::SpriteRenderer(IGraphicsDevice* graphicsDevice, ShaderProgram* spriteShader,
		Texture2D* pureTexture) : _graphicsDevice(graphicsDevice), 
	_spriteShaderPure(spriteShader), _whiteTexture(pureTexture)
{
	_batchState.IsBatchBegin = false;

	graphicsDevice->UseShader(_spriteShaderPure);

	int maxTextureSlots = _graphicsDevice->GetMaxTextureSlots();
	// 纹理列表初始化
	for (int i = 0; i < maxTextureSlots; i++)
	{
		_spriteShaderPure->SetParameter1i(string_format("uTextures[%d]", i).c_str(), i);
	}

	_usedTextures = std::make_unique<const Texture2D*[]>(maxTextureSlots);
	_currentTextureSlots = 0;


	// 预先生成顶点index列表
	_vertexIndices = std::make_unique<unsigned int[]>(MaxIndiciesPerBatch);
	_vertices = std::make_unique<BatchVertex2D[]>(MaxVerticesPerBatch);
	int cur = 0;
	for (int i = 0; i < MaxVerticesPerBatch; i += 4)
	{
		for (int j = 0; j < 6; j++)
		{
			_vertexIndices[cur++] = i + simpleQuadIndicies[j];
		}
	}

	_quadVAO = _graphicsDevice->CreateVertexArray();
	_graphicsDevice->CreateBuffers(2, _quadBuffers);

	_graphicsDevice->BindVertexArray(_quadVAO);
	_graphicsDevice->SetBufferData(BufferType::ARRAY_BUFFER, _quadBuffers[0], 
		sizeof(BatchVertex2D) * MaxVerticesPerBatch, nullptr, BufferHint::DYNAMIC_DRAW);
	_graphicsDevice->SetBufferData(BufferType::INDEX_BUFFER, _quadBuffers[1],
		sizeof(unsigned int) * MaxIndiciesPerBatch, _vertexIndices.get(), BufferHint::STATIC_DRAW);

	VertexLayout vertexLayout;
	vertexLayout.Add(VertexElement(offsetof(BatchVertex2D, Position), 2, EngineDataType::FLOAT, false));
	vertexLayout.Add(VertexElement(offsetof(BatchVertex2D, TextureCoords), 2, EngineDataType::FLOAT, false));
	vertexLayout.Add(VertexElement(offsetof(BatchVertex2D, Color), 4, EngineDataType::UNSIGNED_BYTE, true));
	vertexLayout.Add(VertexElement(offsetof(BatchVertex2D, TextureIndex), 1, EngineDataType::FLOAT, false));
	_graphicsDevice->SetupVertexAttributes(vertexLayout);

	_graphicsDevice->UnbindVertexArray();
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Begin(const glm::mat4& transform,const BatchSettings& settings)
{
	if (_batchState.IsBatchBegin)
	{
		throw std::exception("SpriteRenderer::Begin cannot be called when one is already began");
	}
	_batchState.IsBatchBegin = true;
	_batchState.WorldTransform = transform;
	_batchState.Settings = settings;
	
	if (_batchState.Settings.BlendMode != trv2::BlendingMode::None)
	{
		_graphicsDevice->SetBlendingMode(_batchState.Settings.BlendMode);
	}
	_currentVertex = 0;
}

void SpriteRenderer::End()
{
	if (_currentVertex) flushBatch();
	_batchState.IsBatchBegin = false;
	if (_batchState.Settings.BlendMode != trv2::BlendingMode::None)
	{
		_graphicsDevice->SetBlendingMode(trv2::BlendingMode::None);
	}
}


void SpriteRenderer::Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, 
	const glm::vec4& color, SpriteFlipMode flipMode)
{
	pushTextureQuad(_whiteTexture, pos, size, origin, rotation, color, flipMode, defaultTexCoordsList);
}

void SpriteRenderer::Draw(glm::vec2 pos, glm::vec2 size, const trv2::Rectf& texCoords, glm::vec2 origin, float rotation, const glm::vec4& color, SpriteFlipMode flipMode)
{
	const glm::vec2 texCoordsList[4] = {
		texCoords.BottomLeft(),
		texCoords.BottomRight(),
		texCoords.TopLeft(),
		texCoords.TopRight()
	};
	pushTextureQuad(_whiteTexture, pos, size, origin, rotation, color, flipMode, texCoordsList);
}


void SpriteRenderer::Draw(const Texture2D* texture, glm::vec2 pos, glm::vec2 size,
	glm::vec2 origin, float rotation, const glm::vec4& color,
	SpriteFlipMode flipMode)
{
	if (texture == nullptr) texture = _whiteTexture;
	pushTextureQuad(texture, pos, size, origin, rotation, color, flipMode, defaultTexCoordsList);
}

void SpriteRenderer::Draw(const Texture2D* texture, glm::vec2 pos, glm::vec2 size, const trv2::Rectf& texCoords, glm::vec2 origin, float rotation, const glm::vec4& color, SpriteFlipMode flipMode)
{
	const glm::vec2 texCoordsList[4] = {
		texCoords.BottomLeft(),
		texCoords.BottomRight(),
		texCoords.TopLeft(),
		texCoords.TopRight()
	};
	if (texture == nullptr) texture = _whiteTexture;
	pushTextureQuad(texture, pos, size, origin, rotation, color, flipMode, texCoordsList);
}

void SpriteRenderer::pushTextureQuad(const Texture2D* texture, glm::vec2 tpos, glm::vec2 size, glm::vec2 origin, 
	float rotation, const glm::vec4& color, SpriteFlipMode flipMode, const glm::vec2* useTexCoords)
{
	if (_currentVertex == MaxVerticesPerBatch)
	{
		flushBatch();
	}
	int slotId;
	if ((slotId = findUsedTexture(texture)) == -1)
	{
		if (_currentTextureSlots == _graphicsDevice->GetMaxTextureSlots())
		{
			flushBatch();
		}
		slotId = _currentTextureSlots;
		_usedTextures[_currentTextureSlots++] = texture;
	}

	glm::mat2 transform = glm::identity<glm::mat2>();
	if (rotation != 0.f)
	{
		auto cosr = std::cos(rotation);
		auto sinr = std::sin(rotation);
		transform[0][0] = cosr;
		transform[1][0] = sinr;
		transform[0][1] = -sinr;
		transform[1][1] = cosr;
	}
	auto bColor = vec4ToByteColor(color);

	const unsigned int* texCoordTable = defaultSpriteTexOrder;
	if (flipMode == SpriteFlipMode::FlipHorizontal)
	{
		texCoordTable = flipHorizontalSpriteTexOrder;
	}
	else if (flipMode == SpriteFlipMode::FlipVertical)
	{
		texCoordTable = flipVerticalSpriteTexOrder;
	}
	if (rotation == 0.f)
	{
		auto& curV1 = _vertices[_currentVertex];

		curV1.Position = tpos + (simpleQuadVertices[0].Position - origin) * size;
		curV1.TextureCoords = useTexCoords[texCoordTable[0]];
		curV1.Color = bColor;
		curV1.TextureIndex = (float)slotId;

		auto& curV2 = _vertices[_currentVertex + 1];

		curV2.Position = tpos + (simpleQuadVertices[1].Position - origin) * size;
		curV2.TextureCoords = useTexCoords[texCoordTable[1]];
		curV2.Color = bColor;
		curV2.TextureIndex = (float)slotId;

		auto& curV3 = _vertices[_currentVertex + 2];

		curV3.Position = tpos + (simpleQuadVertices[2].Position - origin) * size;
		curV3.TextureCoords = useTexCoords[texCoordTable[2]];
		curV3.Color = bColor;
		curV3.TextureIndex = (float)slotId;

		auto& curV4 = _vertices[_currentVertex + 3];

		curV4.Position = tpos + (simpleQuadVertices[3].Position - origin) * size;
		curV4.TextureCoords = useTexCoords[texCoordTable[3]];
		curV4.Color = bColor;
		curV4.TextureIndex = (float)slotId;

		_currentVertex += 4;
	}
	else
	{
		auto& curV1 = _vertices[_currentVertex];

		curV1.Position = tpos + transform * ((simpleQuadVertices[0].Position - origin) * size);
		curV1.TextureCoords = useTexCoords[texCoordTable[0]];
		curV1.Color = bColor;
		curV1.TextureIndex = slotId;

		auto& curV2 = _vertices[_currentVertex + 1];

		curV2.Position = tpos + transform * ((simpleQuadVertices[1].Position - origin) * size);
		curV2.TextureCoords = useTexCoords[texCoordTable[1]];
		curV2.Color = bColor;
		curV2.TextureIndex = slotId;

		auto& curV3 = _vertices[_currentVertex + 2];

		curV3.Position = tpos + transform * ((simpleQuadVertices[2].Position - origin) * size);
		curV3.TextureCoords = useTexCoords[texCoordTable[2]];
		curV3.Color = bColor;
		curV3.TextureIndex = slotId;

		auto& curV4 = _vertices[_currentVertex + 3];

		curV4.Position = tpos + transform * ((simpleQuadVertices[3].Position - origin) * size);
		curV4.TextureCoords = useTexCoords[texCoordTable[3]];
		curV4.Color = bColor;
		curV4.TextureIndex = slotId;

		_currentVertex += 4;
	}
}


void SpriteRenderer::flushBatch()
{
	int vertexCount = _currentVertex;
	assert(vertexCount % 4 == 0);

	if (_batchState.Settings.SpriteSortMode == SpriteSortMode::Texture) {
		std::sort(_vertices.get(), _vertices.get() + vertexCount, [](const trv2::BatchVertex2D& v, const trv2::BatchVertex2D& u) {
			return v.TextureIndex < u.TextureIndex;
			});
	}

	_graphicsDevice->BindVertexArray(_quadVAO);


	auto currentShader = _batchState.Settings.Shader ? _batchState.Settings.Shader : _spriteShaderPure;
	_graphicsDevice->UseShader(currentShader);
	// 绑定纹理们
	bindTextures();

	currentShader->SetParameterfm4x4("uWorldTransform", _batchState.WorldTransform);
	// 以最多 MaxVerticesPerBatch 个点为单位，分批绘制线段
	for (int i = 0; i < _currentVertex; i += MaxVerticesPerBatch)
	{
		// 顶点数据输送
		int count = std::min(vertexCount - i, MaxVerticesPerBatch);
		assert(count % 4 == 0);

		_graphicsDevice->ChangeBufferData(BufferType::ARRAY_BUFFER, _quadBuffers[0],
			0, sizeof(BatchVertex2D) * count, _vertices.get());

		// 顶点顺序数据
		int idxCount = count / 4 * 6;
		assert(idxCount % 6 == 0);

		// 绘制
		_graphicsDevice->DrawIndexedPrimitives(PrimitiveType::TRIANGLE_LIST, idxCount, EngineDataType::UNSIGNED_INT, 0);
	}
	_graphicsDevice->UnbindVertexArray();

	// 缓冲区数据清空
	_currentVertex = 0;
	_currentTextureSlots = 0;
}

void SpriteRenderer::bindTextures() const
{
	for (int i = 0; i < _currentTextureSlots; i++)
	{
		_graphicsDevice->BindTexture2DSlot(i, _usedTextures[i]);
	}
}

int SpriteRenderer::findUsedTexture(const Texture2D* texture) const
{
	for (int i = 0; i < _currentTextureSlots; i++)
	{
		if (_usedTextures[i]->GetHandle() == texture->GetHandle())
		{
			return i;
		}
	}
	return -1;
}
TRV2_NAMESPACE_END