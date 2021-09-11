#pragma once
#include <array>
#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>


TRV2_NAMESPACE_BEGIN

class OpenGLProvider : public IGraphicsProvider
{
public:
	OpenGLProvider(const EngineSettings& settings);

	IGraphicsDevice* GetCurrentDeivce() const override { return trv2::ptr(_graphicsDevice); }
	IGraphicsResourceManager* GetGraphicsResourceManager() const override { return trv2::ptr(_graphicsResourceManager); }

	static int MapTextureWarpMethod(TextureWarpMethod warpMethod); 
	static std::array<int, 2> MapTextureSampleMethod(TextureSampleMethod sampleMethod); 
	static int MapPixelFormat(PixelFormat format);
	static int MapShaderType(ShaderType type);
	static int MapDataType(EngineDataType type);
	static int MapBufferType(BufferType type);
	static int MapDrawPrimitivesType(PrimitiveType type);

private:
	std::shared_ptr<IGraphicsDevice> _graphicsDevice;
	std::shared_ptr<IGraphicsResourceManager> _graphicsResourceManager;
};


TRV2_NAMESPACE_END