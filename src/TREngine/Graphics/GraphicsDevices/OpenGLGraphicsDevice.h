#pragma once
#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>

TRV2_NAMESPACE_BEGIN
/**
 * @brief Graphics device object for OpenGL API
*/
class OpenGLGraphicsDevice : public IGraphicsDevice
{
public:
	OpenGLGraphicsDevice(const EngineSettings& clientConfig);
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

	virtual void DrawIndexedPrimitives(PrimitiveType type, size_t count, EngineDataType dataType, size_t offset) override;
	virtual void DrawPrimitives(PrimitiveType type, size_t count, size_t offset) override;

	virtual void BindTexture2DSlot(int slot, const Texture2D* texture) override;

	virtual void SwitchRenderTarget(const RenderTarget2D* renderTarget) override;
	virtual void SetViewPort(int x, int y, int width, int height) override;
	virtual void Clear(const glm::vec4& color) override;

	virtual void UseShader(const ShaderProgram* shader) override;

	virtual int GetMaxTextureSlots() override { return _MaxTextureSlotCanUse; }

	virtual void SetBlendingMode(BlendingMode mode) override;

	virtual void SetDepthTestingMode(DepthTestingMode mode, DepthTestingFunction func) override;

	virtual void SetCullingMode(CullingMode mode) override;

	virtual void SetPolygonMode(PolygonMode mode) override;

	virtual byte_color ReadPixelFromTexture(const Texture2D* texture, int x, int y) override;
private:
	void initializeConstants();

	int _MaxTextureSlotCanUse;
};
TRV2_NAMESPACE_END