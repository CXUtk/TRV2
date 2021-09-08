#pragma once
#include <Graphics/Graphics_Interfaces.h>

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

	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset);

	virtual void BindTexture2DSlot(int slot, const ITexture2D* texture);

	virtual int GetMaxTextureSlots() { return _MaxTextureSlotCanUse; }
private:
	void initializeConstants();

	int _MaxTextureSlotCanUse;
};
TRV2_NAMESPACE_END