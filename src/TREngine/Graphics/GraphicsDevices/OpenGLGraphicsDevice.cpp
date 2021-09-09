#include "OpenGLGraphicsDevice.hpp"

#include <glad/glad.h>
#include <array>
#include <thread>
#include <exception>

#include <TREngine.hpp>
#include <Configs/EngineSettings.hpp>
#include <Graphics/Graphics_Interfaces.hpp>

#include <Graphics/Structures/VertexLayout.hpp>
#include <Graphics/Textures/OpenGLTexture2D.hpp>


TRV2_NAMESPACE_BEGIN


void OpenGLGraphicsDevice::initializeConstants() 
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_MaxTextureSlotCanUse);
}

OpenGLGraphicsDevice::OpenGLGraphicsDevice(const EngineSettings& clientConfig)
{
	initializeConstants();
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
}

void OpenGLGraphicsDevice::SetupVertexAttributes(const VertexLayout& layout)
{
	auto& elements = layout.GetVertexElements();
	int counter = 0;
	for (auto& element : elements)
	{
		glVertexAttribPointer(counter, element.NumberOfElements, _OpenGLAPI::MapDataType(element.DataType), 
			element.Normalized, layout.GetSize(), (void*)element.Offset);
		glEnableVertexAttribArray(counter);
		++counter;
	}
}

IVertexBufferHandle OpenGLGraphicsDevice::CreateVertexArray()
{
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	return vertexArray;
}

IVertexBufferHandle OpenGLGraphicsDevice::CreateBuffer()
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	return buffer;
}

void OpenGLGraphicsDevice::CreateVertexArrays(int size, IVertexBufferHandle* hOut)
{
	glGenVertexArrays(size, hOut);
}

void OpenGLGraphicsDevice::CreateBuffers(int size, IVertexBufferHandle* hOut) 
{
	glGenBuffers(size, hOut);
}

void OpenGLGraphicsDevice::SetBufferData(BufferType type, IVertexBufferHandle handle, size_t size, const void* data, BufferHint bufferHint)
{
	auto bufferType = _OpenGLAPI::MapBufferType(type);
	glBindBuffer(bufferType, handle);
	glBufferData(bufferType, size, data,
		(bufferHint == BufferHint::DYNAMIC_DRAW) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void OpenGLGraphicsDevice::ChangeBufferData(BufferType type, IVertexBufferHandle handle, size_t offset, size_t size, const void* data)
{
	auto target = _OpenGLAPI::MapBufferType(type);
	glBindBuffer(target, handle);
	glBufferSubData(target, offset, size, data);
}

void OpenGLGraphicsDevice::BindBuffer(BufferType type, IVertexBufferHandle handle)
{
	glBindBuffer(_OpenGLAPI::MapBufferType(type), handle);
}

void OpenGLGraphicsDevice::BindVertexArray(IVertexBufferHandle handle)
{
	glBindVertexArray(handle);
}

void OpenGLGraphicsDevice::UnbindVertexArray()
{
	glBindVertexArray(0);
}

void OpenGLGraphicsDevice::DrawIndexedPrimitives(PrimitiveType type, size_t count, EngineDataType dataType, size_t offset)
{
	glDrawElements(_OpenGLAPI::MapDrawPrimitivesType(type), count, _OpenGLAPI::MapDataType(dataType), (void*) offset);
}

void OpenGLGraphicsDevice::BindTexture2DSlot(int slot, const ITexture2D* texture)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
}
TRV2_NAMESPACE_END