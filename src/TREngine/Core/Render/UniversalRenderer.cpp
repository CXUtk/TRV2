#include "UniversalRenderer.h"

#include <glm/gtx/transform.hpp>
#include <algorithm>
#include <Engine.h>

#include <Core/Utils/Utils.h>
#include <Core/Assets/AssetsManager.h>
#include <Core/Structures/VertexLayout.h>

#include "ShaderProgram.h"


TRV2_NAMESPACE_BEGIN
UniversalRenderer::UniversalRenderer()
{
	_assetManager = Engine::GetInstance()->GetAssetsManager();
	_graphicsDevice = Engine::GetInstance()->GetGraphicsDevice();


	_pureColorShader = _assetManager->GetShader("builtin::pure");


	_vao = _graphicsDevice->CreateVertexArray();
	_vbo = _graphicsDevice->CreateBuffer();

	_graphicsDevice->BindVertexArray(_vao);
	_graphicsDevice->BindBuffer(trv2::BufferType::ARRAY_BUFFER, _vbo);

	VertexLayout vertexLayout;
	vertexLayout.Add(VertexElement(offsetof(Vertex, Position), 2, EngineDataType::FLOAT, false));
	vertexLayout.Add(VertexElement(offsetof(Vertex, Color), 4, EngineDataType::FLOAT, false));
	_graphicsDevice->SetupVertexAttributes(vertexLayout);
	_graphicsDevice->UnbindVertexArray();

}

UniversalRenderer::~UniversalRenderer()
{}
void UniversalRenderer::DrawPoint(glm::vec2 point)
{
	_vertexBuffer.push_back(Vertex{ point, glm::vec4(1) });
}
void UniversalRenderer::DrawLine(glm::vec2 start, glm::vec2 end)
{
	_vertexBuffer.push_back(Vertex{ start, glm::vec4(1) });
	_vertexBuffer.push_back(Vertex{ end, glm::vec4(1) });
}
void UniversalRenderer::DrawLine(glm::vec2 start, glm::vec2 end, const glm::vec4& color1, const glm::vec4& color2)
{
	_vertexBuffer.push_back(Vertex{ start, color1 });
	_vertexBuffer.push_back(Vertex{ end, color2 });
}
void UniversalRenderer::DrawWiredTriangle(glm::vec2 A, glm::vec2 B, glm::vec2 C)
{
	_vertexBuffer.push_back(Vertex{ A, glm::vec4(1) });
	_vertexBuffer.push_back(Vertex{ B, glm::vec4(1) });
	_vertexBuffer.push_back(Vertex{ C, glm::vec4(1) });
}
void UniversalRenderer::DrawWiredTriangle(glm::vec2 A, glm::vec2 B, glm::vec2 C, const glm::vec4& color)
{
	_vertexBuffer.push_back(Vertex{ A, color });
	_vertexBuffer.push_back(Vertex{ B, color });
	_vertexBuffer.push_back(Vertex{ C, color });
}
void UniversalRenderer::DrawWiredTriangle(const Vertex& A, const Vertex& B, const Vertex& C)
{
	_vertexBuffer.push_back(A);
	_vertexBuffer.push_back(B);
	_vertexBuffer.push_back(C);
}

void UniversalRenderer::SetPolygonMode(trv2::PolygonMode mode)
{
	_graphicsDevice->SetPolygonMode(mode);
}

void UniversalRenderer::Flush(PrimitiveType primitiveType, const glm::mat4& projection)
{
	_graphicsDevice->BindVertexArray(_vao);
	_graphicsDevice->UseShader(_pureColorShader);

	_pureColorShader->SetParameterfm4x4("uWorldTransform", projection);

	_graphicsDevice->SetBufferData(trv2::BufferType::ARRAY_BUFFER, _vbo,
		sizeof(Vertex) * _vertexBuffer.size(), _vertexBuffer.data(), BufferHint::DYNAMIC_DRAW);

	switch (primitiveType)
	{
	case trv2::PrimitiveType::TRIANGLE_LIST:
	{
		_graphicsDevice->DrawPrimitives(primitiveType, _vertexBuffer.size(), 0);
		break;
	}
	case trv2::PrimitiveType::TRIANGLE_STRIP:
		break;
	case trv2::PrimitiveType::LINE_LIST:
	{
		_graphicsDevice->DrawPrimitives(primitiveType, _vertexBuffer.size(), 0);
		break;
	}
	case trv2::PrimitiveType::LINE_STRIP:
		break;
	case trv2::PrimitiveType::POINTS:
		_graphicsDevice->DrawPrimitives(primitiveType, _vertexBuffer.size(), 0);
		break;
	default:
		break;
	}

	_graphicsDevice->UnbindVertexArray();

	_vertexBuffer.clear();
}
TRV2_NAMESPACE_END