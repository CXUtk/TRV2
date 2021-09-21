#pragma once

#include <Core.h>
#include <Graphics/Graphics_Interfaces.h>
#include <string>

TRV2_NAMESPACE_BEGIN
/**
 * @brief Graphics device object for OpenGL API
*/
class OpenGLGraphicsResourceManager : public IGraphicsResourceManager
{
public:
	explicit OpenGLGraphicsResourceManager(const EngineSettings& clientConfig);
	~OpenGLGraphicsResourceManager() override;

	virtual ITextureHandle CreateTexture2D(glm::ivec2 size,
		const TextureParameters& parameters, PixelFormat srcFormat, 
		EngineDataType dataType, const void* data) override;
	virtual void ChangeTexture2D(ITextureHandle handle, glm::ivec2 size,
		const TextureParameters& parameters, PixelFormat srcFormat,
		EngineDataType dataType, const void* data) override;
	virtual void DeleteTexture2D(ITextureHandle handle) override;

	virtual IShaderHandle CreateRawShader(const char* code, ShaderType shaderType, const char* fileName) override;
	virtual void DeleteRawShader(IShaderHandle handle) override;

	virtual IShaderProgramHandle CreateShaderProgram(const std::vector<const RawShader*>& shaders) override;
	virtual void DeleteShaderProgram(IShaderProgramHandle handle) override;

	virtual IRenderTarget2DHandle CreateRenderTarget2D(Texture2D* receiver, glm::ivec2 size) override;
	virtual void DeleteRenderTarget2D(IRenderTarget2DHandle handle) override;

	virtual void SetShaderParameterMat4x4(ShaderProgram* shader, const std::string& name, const glm::mat4& mat, bool normalized) override;
	virtual void SetShaderParameter1i(ShaderProgram* shader, const std::string& name, int value) override;
private:
};
TRV2_NAMESPACE_END