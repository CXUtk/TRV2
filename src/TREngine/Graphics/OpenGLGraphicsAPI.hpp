#pragma once
#include <TREngine_Interfaces.h>
//#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
//#include <Graphics/Shaders/OpenGLRawShader.h>
//#include <Graphics/Shaders/OpenGLShaderProgram.h>
//#include <Graphics/Textures/OpenGLTexture2D.h>

TRV2_NAMESPACE_BEGIN

class OpenGLGraphicsDevice;
class OpenGLShaderProgram;
class OpenGLRawShader;
class OpenGLTexture2D;

class _OpenGLAPI
{
public:
	// 基础图形资源的映射类
	using _GraphicsDevice_Type = OpenGLGraphicsDevice;
	using _ShaderProgram_Type = OpenGLShaderProgram;
	using _RawShader_Type = OpenGLRawShader;
	using _Texture2D_Type = OpenGLTexture2D;

	enum { APIType = GraphicsAPIType::OpenGL };

	_OpenGLAPI(const EngineSettings& settings);

	_GraphicsDevice_Type* GetGraphicsDevice() const { return _graphicsDevice.get(); }

private:
	std::shared_ptr<_GraphicsDevice_Type> _graphicsDevice;
};

using _G_API = _OpenGLAPI;
TRV2_NAMESPACE_END