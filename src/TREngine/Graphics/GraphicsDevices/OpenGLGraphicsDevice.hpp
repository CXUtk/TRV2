#pragma once
#include <Graphics/OpenGLGraphicsAPI.hpp>
#include <Core.hpp>

TRV2_NAMESPACE_BEGIN
/**
 * @brief Graphics device object for OpenGL API
*/
class OpenGLGraphicsDevice {
public:
	OpenGLGraphicsDevice(const EngineSettings& clientConfig);
	~OpenGLGraphicsDevice();

	void SetupVertexAttributes(const VertexLayout& layout);

	virtual IVertexBufferHandle CreateVertexArray();
	virtual IVertexBufferHandle CreateBuffer();

	virtual void CreateVertexArrays(int size, IVertexBufferHandle* hOut);
	virtual void CreateBuffers(int size, IVertexBufferHandle* hOut);

	virtual void SetBufferData(BufferType type, IVertexBufferHandle handle, size_t size,
		const void* data, BufferHint bufferHint);
	virtual void ChangeBufferData(BufferType type, IVertexBufferHandle handle,
		size_t offset, size_t size, const void* data);

	virtual void BindBuffer(BufferType type, IVertexBufferHandle handle);

	virtual void BindVertexArray(IVertexBufferHandle handle);
	virtual void UnbindVertexArray();

	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, EngineDataType dataType, size_t offset);

	virtual void BindTexture2DSlot(int slot, const OpenGLTexture2D* texture);

	virtual int GetMaxTextureSlots() { return _MaxTextureSlotCanUse; }

	virtual void SwitchRenderTarget(const OpenGLRenderTarget2D* renderTarget);
	virtual void SetViewPort(int x, int y, int width, int height);
	virtual void Clear(const glm::vec4& color);
private:
	void initializeConstants();

	int _MaxTextureSlotCanUse;
};
TRV2_NAMESPACE_END