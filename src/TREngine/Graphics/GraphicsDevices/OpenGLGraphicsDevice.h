﻿#pragma once
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

	void SetupVertexAttributes(const VertexLayout& layout) const override;

	virtual IVertexBufferHandle CreateVertexArray() const override;
	virtual IVertexBufferHandle CreateBuffer() const override;

	virtual void CreateVertexArrays(int size, IVertexBufferHandle* hOut) const override;
	virtual void CreateBuffers(int size, IVertexBufferHandle* hOut) const override;

	virtual void SetBufferData(BufferType type, IVertexBufferHandle handle, size_t size,
		const void* data, BufferHint bufferHint) const override;
	virtual void ChangeBufferData(BufferType type, IVertexBufferHandle handle,
		size_t offset, size_t size, const void* data) const override;

	virtual void BindBuffer(BufferType type, IVertexBufferHandle handle) const override;

	virtual void BindVertexArray(IVertexBufferHandle handle) const override;
	virtual void UnbindVertexArray() const override;

	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, BufferDataType dataType, size_t offset) const override;

	virtual void BindTexture2DSlot(int slot, const ITexture2D* textureHandle) const override;

	virtual int GetMaxTextureSlots() const override { return _MaxTextureSlotCanUse; }
private:
	void initializeConstants();

	int _MaxTextureSlotCanUse;
};
TRV2_NAMESPACE_END