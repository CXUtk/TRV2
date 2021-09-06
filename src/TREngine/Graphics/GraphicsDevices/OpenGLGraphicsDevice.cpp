#include "OpenGLGraphicsDevice.h"


#include <exception>
#include <array>
#include <TREngine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Configs/EngineSettings.h>
#include <Assets/AssetsManager.h>
#include <Graphics/Structures/VertexLayout.h>

TRV2_NAMESPACE_BEGIN
template<int T>
constexpr std::array<int, T> generateDataTypeMapper()
{
	std::array<int, T> M{0};
	M[(int)BufferDataType::FLOAT] = GL_FLOAT;
	M[(int)BufferDataType::INT] = GL_INT;
	M[(int)BufferDataType::UNSIGNED_INT] = GL_UNSIGNED_INT;
	M[(int)BufferDataType::SHORT] = GL_SHORT;
	M[(int)BufferDataType::UNSIGNED_SHORT] = GL_UNSIGNED_SHORT;
	M[(int)BufferDataType::BYTE] = GL_BYTE;
	M[(int)BufferDataType::UNSIGNED_BYTE] = GL_UNSIGNED_BYTE;
	return M;
}

template<int T>
constexpr std::array<int, T> generateBufferTypeMapper()
{
	std::array<int, T> M{ 0 };
	M[(int)BufferType::ARRAY_BUFFER] = GL_ARRAY_BUFFER;
	M[(int)BufferType::INDEX_BUFFER] = GL_ELEMENT_ARRAY_BUFFER;
	return M;
}

template<int T>
constexpr std::array<int, T> generatePrimitiveTypeMapper()
{
	std::array<int, T> M{ 0 };
	M[(int)PrimitiveType::TRIANGLE_LIST] = GL_TRIANGLES;
	M[(int)PrimitiveType::TRIANGLE_STRIP] = GL_TRIANGLE_STRIP;
	M[(int)PrimitiveType::LINE_LIST] = GL_LINES;
	M[(int)PrimitiveType::LINE_STRIP] = GL_LINE_STRIP;
	M[(int)PrimitiveType::POINTS] = GL_POINTS;
	return M;
}

static constexpr auto DataTypeToGLMapper = generateDataTypeMapper<(int)BufferDataType::__COUNT>();
static constexpr auto BufferTypeMapper = generateBufferTypeMapper<(int)BufferType::__COUNT>();
static constexpr auto DrawPrimitivesTypeMapper = generatePrimitiveTypeMapper<(int)PrimitiveType::__COUNT>();

void OpenGLGraphicsDevice::initializeConstants()
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_MaxTextureSlotCanUse);
}

OpenGLGraphicsDevice::OpenGLGraphicsDevice(const EngineSettings* clientConfig) : IGraphicsDevice(clientConfig)
{
	initializeConstants();
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
}

void OpenGLGraphicsDevice::SetupVertexAttributes(const VertexLayout& layout) const
{
	auto& elements = layout.GetVertexElements();
	int counter = 0;
	for (auto& element : elements)
	{
		glVertexAttribPointer(counter, element.NumberOfElements, DataTypeToGLMapper[(int)element.DataType], 
			GL_FALSE, layout.GetSize(), (void*)element.Offset);
		glEnableVertexAttribArray(counter);
		++counter;
	}
}
IVertexBufferHandle OpenGLGraphicsDevice::CreateVertexArray() const
{
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	return vertexArray;
}
IVertexBufferHandle OpenGLGraphicsDevice::CreateBuffer() const
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	return buffer;
}
void OpenGLGraphicsDevice::CreateVertexArrays(int size, IVertexBufferHandle* hOut) const
{
	glGenVertexArrays(size, hOut);
}

void OpenGLGraphicsDevice::CreateBuffers(int size, IVertexBufferHandle* hOut) const
{
	glGenBuffers(size, hOut);
}

void OpenGLGraphicsDevice::SetBufferData(BufferType type, IVertexBufferHandle handle, size_t size, const void* data, BufferHint bufferHint) const
{
	glBindBuffer(BufferTypeMapper[(int)type], handle);
	glBufferData(BufferTypeMapper[(int)type], size, data, 
		(bufferHint == BufferHint::DYNAMIC_DRAW) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void OpenGLGraphicsDevice::ChangeBufferData(BufferType type, IVertexBufferHandle handle, size_t offset, size_t size, const void* data) const
{
	glBindBuffer(BufferTypeMapper[(int)type], handle);
	glBufferSubData(BufferTypeMapper[(int)type], offset, size, data);
}

void OpenGLGraphicsDevice::BindBuffer(BufferType type, IVertexBufferHandle handle) const
{
	glBindBuffer(BufferTypeMapper[(int)type], handle);
}

void OpenGLGraphicsDevice::BindVertexArray(IVertexBufferHandle handle) const
{
	glBindVertexArray(handle);
}

void OpenGLGraphicsDevice::UnbindVertexArray() const
{
	glBindVertexArray(0);
}
void OpenGLGraphicsDevice::DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset) const
{
	glDrawElements(DrawPrimitivesTypeMapper[(int)type], count, DataTypeToGLMapper[(int)dataType], (void*) offset);
}
void OpenGLGraphicsDevice::BindTexture2DSlot(int slot, const ITexture2D* texture) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
}
TRV2_NAMESPACE_END