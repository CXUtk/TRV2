#pragma once

#include <vector>
#include <memory>
#include <map>

#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>
#include <Core/Structures/Rect.hpp>

TRV2_NAMESPACE_BEGIN

struct Vertex
{
	glm::vec2 Position;
	glm::vec4 Color;
};

class UniversalRenderer
{
public:
	UniversalRenderer();
	~UniversalRenderer();

	void DrawPoint(glm::vec2 point);

	void DrawLine(glm::vec2 start, glm::vec2 end);
	void DrawLine(glm::vec2 start, glm::vec2 end, const glm::vec4& color1, const glm::vec4& color2);

	void DrawWiredTriangle(glm::vec2 A, glm::vec2 B, glm::vec2 C);
	void DrawWiredTriangle(glm::vec2 A, glm::vec2 B, glm::vec2 C, const glm::vec4& color);
	void DrawWiredTriangle(const Vertex& A, const Vertex& B, const Vertex& C);

	void SetPolygonMode(trv2::PolygonMode mode);

	void Flush(PrimitiveType primitiveType, const glm::mat4& projection);


private:
	IVertexBufferHandle _vao;
	IVertexBufferHandle _vbo;

	ShaderProgram* _pureColorShader;
	IGraphicsDevice* _graphicsDevice;
	AssetsManager* _assetManager;

	std::vector<Vertex> _vertexBuffer;
};

TRV2_NAMESPACE_END