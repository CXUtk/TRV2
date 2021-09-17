#include "OpenGLGraphicsDevice.h"

#include <glad/glad.h>
#include <array>
#include <thread>
#include <exception>

#include <Engine.h>
#include <Core/Structures/EngineSettings.h>
#include <Core/Render/Texture2D.h>
#include <Core/Render/RenderTarget2D.h>
#include <Core/Render/ShaderProgram.h>
#include <Platform/Platform_Interfaces.h>

#include <Graphics/Graphics_Interfaces.h>
#include <Graphics/Structures/VertexLayout.hpp>
#include <Graphics/OpenGLProvider.h>


TRV2_NAMESPACE_BEGIN


void OpenGLGraphicsDevice::SwitchRenderTarget(const RenderTarget2D* renderTarget)
{
	if (renderTarget == nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		auto windowSize = Engine::GetInstance()->GetGameWindow()->GetWindowSize();
		glViewport(0, 0, windowSize.x, windowSize.y);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->GetHandle());

	auto size = renderTarget->GetSize();
	glViewport(0, 0, size.x, size.y);
}

void OpenGLGraphicsDevice::SetViewPort(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void OpenGLGraphicsDevice::Clear(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLGraphicsDevice::UseShader(const ShaderProgram* shader)
{
	glUseProgram(shader->GetHandle());
}

void OpenGLGraphicsDevice::SetBlendingMode(BlendingMode mode)
{
	if (mode == BlendingMode::AlphaBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}

void OpenGLGraphicsDevice::SetDepthTestingMode(DepthTestingMode mode, DepthTestingFunction func)
{
	if (mode == DepthTestingMode::None)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask((mode == DepthTestingMode::DepthTestNoApply) ? GL_FALSE : GL_TRUE);

		glDepthFunc(OpenGLProvider::MapDepthTestingFunctionType(func));
	}
}

void OpenGLGraphicsDevice::SetCullingMode(CullingMode mode)
{
	if (mode == CullingMode::None)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(mode == CullingMode::CullCW ? GL_CW : GL_CCW);
	}
}

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
		glVertexAttribPointer(counter, element.NumberOfElements, OpenGLProvider::MapDataType(element.DataType),
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
	auto bufferType = OpenGLProvider::MapBufferType(type);
	glBindBuffer(bufferType, handle);
	glBufferData(bufferType, size, data,
		(bufferHint == BufferHint::DYNAMIC_DRAW) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void OpenGLGraphicsDevice::ChangeBufferData(BufferType type, IVertexBufferHandle handle, size_t offset, size_t size, const void* data)
{
	auto target = OpenGLProvider::MapBufferType(type);
	glBindBuffer(target, handle);
	glBufferSubData(target, offset, size, data);
}

void OpenGLGraphicsDevice::BindBuffer(BufferType type, IVertexBufferHandle handle)
{
	glBindBuffer(OpenGLProvider::MapBufferType(type), handle);
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
	glDrawElements(OpenGLProvider::MapDrawPrimitivesType(type), count, OpenGLProvider::MapDataType(dataType), (void*) offset);
}

void OpenGLGraphicsDevice::BindTexture2DSlot(int slot, const Texture2D* texture)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
}
TRV2_NAMESPACE_END