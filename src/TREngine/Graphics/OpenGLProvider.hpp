#pragma once
#include <Engine_Interfaces.hpp>
#include <Graphics/Graphics_Interfaces.hpp>
#include <array>
TRV2_NAMESPACE_BEGIN

class OpenGLProvider
{
public:
	// 基础图形资源的映射类
	using _GraphicsDevice_Type = OpenGLGraphicsDevice;
	using _ShaderProgram_Type = OpenGLShaderProgram;
	using _RawShader_Type = OpenGLRawShader;
	using _Texture2D_Type = OpenGLTexture2D;
	using _RenderTarget2D_Type = OpenGLRenderTarget2D;

	enum { APIType = GraphicsAPIType::OpenGL };

	OpenGLProvider(const EngineSettings& settings);

	IGraphicsDevice* GetGraphicsDevice() const { return ptr(_graphicsDevice); }

	static int MapTextureWarpMethod(TextureWarpMethod warpMethod); 
	static std::array<int, 2> MapTextureSampleMethod(TextureSampleMethod sampleMethod); 
	static int MapPixelFormat(PixelFormat format);
	static int MapShaderType(ShaderType type);
	static int MapDataType(EngineDataType type);
	static int MapBufferType(BufferType type);
	static int MapDrawPrimitivesType(PrimitiveType type);
private:
	std::shared_ptr<_GraphicsDevice_Type> _graphicsDevice;
};

using _G_API = _OpenGLAPI;

TRV2_NAMESPACE_END