#pragma once
#include <TREngine_Interfaces.hpp>
#include <Graphics/Graphics_Interfaces.hpp>
#include <array>
//#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
//#include <Graphics/Shaders/OpenGLRawShader.h>
//#include <Graphics/Shaders/OpenGLShaderProgram.h>
//#include <Graphics/Textures/OpenGLTexture2D.h>

TRV2_NAMESPACE_BEGIN

class OpenGLGraphicsDevice;
class OpenGLShaderProgram;
class OpenGLRawShader;
class OpenGLTexture2D;
class OpenGLRenderTarget2D;

class _OpenGLAPI
{
public:
	// 基础图形资源的映射类
	using _GraphicsDevice_Type = OpenGLGraphicsDevice;
	using _ShaderProgram_Type = OpenGLShaderProgram;
	using _RawShader_Type = OpenGLRawShader;
	using _Texture2D_Type = OpenGLTexture2D;
	using _RenderTarget2D_Type = OpenGLRenderTarget2D;

	enum { APIType = GraphicsAPIType::OpenGL };

	_OpenGLAPI(const EngineSettings& settings);

	_GraphicsDevice_Type* GetGraphicsDevice() const { return _graphicsDevice.get(); }

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