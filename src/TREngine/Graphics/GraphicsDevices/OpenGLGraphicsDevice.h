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

	std::shared_ptr<ISpriteRenderer> CreateSpriteRenderer() const override;
	void SetupVertexAttributes(const VertexLayout& layout) const override;

	virtual IGraphicsHandle CreateVertexArray() const override;
	virtual IGraphicsHandle CreateBuffer() const override;

	virtual void CreateVertexArrays(int size, IGraphicsHandle* hOut) const override;
	virtual void CreateBuffers(int size, IGraphicsHandle* hOut) const override;

	virtual void SetBufferData(BufferType type, IGraphicsHandle handle, size_t size,
		const void* data, BufferHint bufferHint) const override;
	virtual void ChangeBufferData(BufferType type, IGraphicsHandle handle,
		size_t offset, size_t size, const void* data) const override;

	virtual void BindBuffer(BufferType type, IGraphicsHandle handle) const override;

	virtual void BindVertexArray(IGraphicsHandle handle) const override;
	virtual void UnbindVertexArray() const override;

	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset) const override;

	virtual void BindTexture2DSlot(int slot, IGraphicsHandle textureHandle) const override;

	virtual int GetMaxTextureSlots() const override { return _MaxTextureSlotCanUse; }
private:
	void initializeConstants();

	int _MaxTextureSlotCanUse;
};
TRV2_NAMESPACE_END