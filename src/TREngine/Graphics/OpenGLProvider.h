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
	static int MapDepthTestingFunctionType(DepthTestingFunction func);
private:
	std::shared_ptr<IGraphicsDevice> _graphicsDevice;
	std::shared_ptr<IGraphicsResourceManager> _graphicsResourceManager;
};

//GLenum glCheckError_(const char* file, int line)
//{
//    GLenum errorCode;
//    while ((errorCode = glGetError()) != GL_NO_ERROR)
//    {
//        std::string error;
//        switch (errorCode)
//        {
//        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
//        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
//        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
//        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
//        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
//        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
//        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
//        }
//        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
//    }
//    return errorCode;
//}
//#define glCheckError() glCheckError_(__FILE__, __LINE__) 

TRV2_NAMESPACE_END