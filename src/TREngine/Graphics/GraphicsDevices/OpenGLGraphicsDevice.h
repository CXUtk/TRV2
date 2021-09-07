#pragma once
#include <Graphics/Graphics_Interfaces.h>
#include <Graphics/Structures/VertexLayout.h>

TRV2_NAMESPACE_BEGIN
/**
 * @brief Graphics device object for OpenGL API
*/
class OpenGLGraphicsDevice : public IGraphicsDevice {
public:
	OpenGLGraphicsDevice(const EngineSettings* clientConfig);
	~OpenGLGraphicsDevice() override;

	void SetupVertexAttributes(const VertexLayout& layout) override;

	virtual IVertexBufferHandle CreateVertexArray() override;
	virtual IVertexBufferHandle CreateBuffer() override;

	virtual void CreateVertexArrays(int size, IVertexBufferHandle* hOut) override;
	virtual void CreateBuffers(int size, IVertexBufferHandle* hOut) override;

	virtual void SetBufferData(BufferType type, IVertexBufferHandle handle, size_t size,
		const void* data, BufferHint bufferHint) override;
	virtual void ChangeBufferData(BufferType type, IVertexBufferHandle handle,
		size_t offset, size_t size, const void* data) override;

	virtual void BindBuffer(BufferType type, IVertexBufferHandle handle) override;

	virtual void BindVertexArray(IVertexBufferHandle handle) override;
	virtual void UnbindVertexArray() override;

	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset) override;

	virtual void BindTexture2DSlot(int slot, const ITexture2D* textureHandle) override;

	virtual int GetMaxTextureSlots() override { return _MaxTextureSlotCanUse; }
private:
	void initializeConstants();

	int _MaxTextureSlotCanUse;
};
TRV2_NAMESPACE_END