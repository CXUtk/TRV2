#include "OpenGLSpriteRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <Utils/Logging/Logger.h>
#include <Assets/Loaders/OpenGLShaderLoader.h>
#include <Assets/Loaders/OpenGLTextureLoader.h>
#include <Utils/Utils.h>

TRV2_NAMESPACE_BEGIN
static const BatchVertex2D simpleQuadVertices[4] = {
	BatchVertex2D(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec4(1)),
	BatchVertex2D(glm::vec2(1, 0), glm::vec2(1, 0), glm::vec4(1)),
	BatchVertex2D(glm::vec2(0, 1), glm::vec2(0, 1), glm::vec4(1)),
	BatchVertex2D(glm::vec2(1, 1), glm::vec2(1, 1), glm::vec4(1))
};

static const GLuint simpleQuadIndicies[6] = { 0, 1, 3, 0, 3, 2 };
static constexpr int MaxQuadsPerBatch = 16384;
static constexpr int MaxVerticesPerBatch = MaxQuadsPerBatch * 4;
static constexpr int MaxIndiciesPerBatch = MaxQuadsPerBatch * 6;

OpenGLSpriteRenderer::OpenGLSpriteRenderer(const ITRGraphicsDevice* graphicsDevice)
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);

	// 初始化资源
	_spriteShaderPure = OpenGLShaderLoader::LoadOpenGLShader("Resources/Shaders/sprite2d.vert",
			"Resources/Shaders/sprite2d.frag");
	int whitePixel = 0xffffffff;
	_whiteTexture = OpenGLTextureLoader::CreateTexture2D(1, 1, (unsigned char*)&whitePixel);

	_textureSlotsBuffer = std::make_unique<float[]>(_maxTextureUnits);
	_spriteShaderPure->Apply();
	for (int i = 0; i < _maxTextureUnits; i++) {
		_spriteShaderPure->SetParameteri1(string_format("uTextures[%d]", i).c_str(), i);
	}


	// 预先生成顶点index列表
	_vertexIndices = std::make_unique<GLuint[]>(MaxIndiciesPerBatch);
	_vertices = std::make_unique<BatchVertex2D[]>(MaxVerticesPerBatch);
	int cur = 0;
	for (int i = 0; i < MaxVerticesPerBatch; i += 4) {
		for (int j = 0; j < 6; j++) {
			_vertexIndices[cur++] = i + simpleQuadIndicies[j];
		}
	}

	glGenVertexArrays(1, &_mainVAO);
	glGenBuffers(1, &_mainVBO);
	glGenBuffers(1, &_mainEBO);

	glBindVertexArray(_mainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _mainVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BatchVertex2D) * MaxVerticesPerBatch, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mainEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * MaxIndiciesPerBatch, _vertexIndices.get(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex2D), (void*)offsetof(BatchVertex2D, Position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex2D), (void*)offsetof(BatchVertex2D, TextureCoords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(BatchVertex2D), (void*)offsetof(BatchVertex2D, Color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(BatchVertex2D), (void*)offsetof(BatchVertex2D, TextureIndex));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	if (_currentVertex) flushBatch();
	_batchStateStack.pop_back();
}

void OpenGLSpriteRenderer::Draw(glm::vec2 pos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color)
{
	pushTextureQuad(trv2::cref(_whiteTexture), pos, size, origin, rotation, color);
}

void OpenGLSpriteRenderer::Draw(const ITexture2D& texture, glm::vec2 pos, glm::vec2 size,
	glm::vec2 origin, float rotation, const glm::vec4& color)
{
	pushTextureQuad(texture, pos, size, origin, rotation, color);
}

glm::mat4 OpenGLSpriteRenderer::getCurrentTransform() const
{
	return _batchStateStack.back().WorldTransform;
}

void OpenGLSpriteRenderer::pushTextureQuad(const ITexture2D& texture, glm::vec2 tpos, glm::vec2 size, glm::vec2 origin, float rotation, const glm::vec4& color)
{

	if (_currentVertex == MaxVerticesPerBatch) {
		flushBatch();
	}
	auto texId = texture.GetId();
	if (_usedTextures.find(texId) == _usedTextures.end()) {
		if (_textureRefs.size() == _maxTextureUnits) {
			flushBatch();
		}
		_usedTextures[texId] = _textureRefs.size();
		_textureRefs.push_back(&texture);
	}
	int slotId = _usedTextures[texId];
	glm::mat2 transform = glm::identity<glm::mat2>();
	if (rotation != 0.f) {
		auto cosr = std::cos(rotation);
		auto sinr = std::sin(rotation);
		transform[0][0] = cosr;
		transform[1][0] = sinr;
		transform[0][1] = -sinr;
		transform[1][1] = cosr;
	}
	for (int i = 0; i < 4; i++) {
		auto pos = (simpleQuadVertices[i].Position - origin) * size;
		auto vpos = (rotation == 0.f) ? (pos) : (transform * pos);

		auto& curV = _vertices[_currentVertex];
		curV.Position.x = vpos.x + tpos.x;
		curV.Position.y = vpos.y + tpos.y;
		curV.TextureCoords = simpleQuadVertices[i].TextureCoords;
		curV.Color = color;
		curV.TextureIndex = slotId;

		_currentVertex++;
	}
}


void OpenGLSpriteRenderer::flushBatch()
{

	{
		int sz = _currentVertex;
		assert(sz % 4 == 0);
		glBindVertexArray(_mainVAO);
		_spriteShaderPure->Apply();
		// 绑定纹理们
		int slots = 0;
		for (auto& tex : _textureRefs) {
			tex->Bind(slots++);
		}
		_spriteShaderPure->SetParameterfm4x4("uWorldTransform", getCurrentTransform());
		// 以最多 MaxVerticesPerBatch 个点为单位，分批绘制线段
		for (int i = 0; i < _currentVertex; i += MaxVerticesPerBatch) {

			// 顶点数据输送
			glBindBuffer(GL_ARRAY_BUFFER, _mainVBO);
			int count = std::min(sz - i, MaxVerticesPerBatch);
			assert(count % 4 == 0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BatchVertex2D) * count, _vertices.get());

			// 顶点顺序数据
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mainEBO);
			int idxCount = count / 4 * 6;
			assert(idxCount % 6 == 0);

			// 绘制
			glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}

	// 缓冲区数据清空
	_currentVertex = 0;
	_usedTextures.clear();
	_textureRefs.clear();
}
TRV2_NAMESPACE_END