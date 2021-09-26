#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "OpenGLProvider.h"
#include <Engine.h>
#include <Core/Utils/Utils.h>
#include <Core/Utils/Logging/Logger.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/ResourceManager/OpenGLGraphicsResourceManager.h>


TRV2_NAMESPACE_BEGIN

template<size_t T>
static constexpr std::array<int, T> generateTextureWarpMethod()
{
    std::array<int, T> M{ 0 };
    M[(int)TextureWarpMethod::REPEAT] = GL_REPEAT;
    M[(int)TextureWarpMethod::MIRRORED_REPEAT] = GL_MIRRORED_REPEAT;
    M[(int)TextureWarpMethod::CLAMP_TO_EDGE] = GL_CLAMP_TO_EDGE;
    return M;
}

template<size_t T>
static constexpr std::array<int, T> generatePixelFormat()
{
    std::array<int, T> M{ 0 };
    M[(int)PixelFormat::RED] = GL_RED;
    M[(int)PixelFormat::RG] = GL_RG;
    M[(int)PixelFormat::RGB] = GL_RGB;
    M[(int)PixelFormat::RGBA] = GL_RGBA;
    M[(int)PixelFormat::RGB16F] = GL_RGB16F;
    M[(int)PixelFormat::DEPTH] = GL_DEPTH_COMPONENT;
    M[(int)PixelFormat::STENCIL] = GL_STENCIL_INDEX;
    M[(int)PixelFormat::DEPTH_STENCIL] = GL_DEPTH_STENCIL;
    return M;
}

template<size_t T>
static constexpr std::array<std::array<int, 2>, T> generateTextureSampleMethod()
{
    std::array<std::array<int, 2>, T> M{ };
    M[(int)TextureSampleMethod::NEAREST] = { GL_NEAREST, GL_NEAREST };
    M[(int)TextureSampleMethod::BI_LINEAR] = { GL_LINEAR, GL_LINEAR };
    M[(int)TextureSampleMethod::BI_LINEAR_MIPMAP] = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
    return M;
}

template<size_t T>
constexpr std::array<int, T> generateShaderTypeMapper()
{
    std::array<int, T> M{ 0 };

    M[(int)ShaderType::VERTEX_SHADER] = GL_VERTEX_SHADER;
    M[(int)ShaderType::FRAGMENT_SHADER] = GL_FRAGMENT_SHADER;
    M[(int)ShaderType::GEOMETRY_SHADER] = GL_GEOMETRY_SHADER;
    M[(int)ShaderType::COMPUTE_SHADER] = GL_COMPUTE_SHADER;
    return M;
}
template<size_t T>
constexpr std::array<int, T> generateDataTypeMapper()
{
    std::array<int, T> M{ 0 };
    M[(int)EngineDataType::FLOAT] = GL_FLOAT;
    M[(int)EngineDataType::INT] = GL_INT;
    M[(int)EngineDataType::UNSIGNED_INT] = GL_UNSIGNED_INT;
    M[(int)EngineDataType::SHORT] = GL_SHORT;
    M[(int)EngineDataType::UNSIGNED_SHORT] = GL_UNSIGNED_SHORT;
    M[(int)EngineDataType::BYTE] = GL_BYTE;
    M[(int)EngineDataType::UNSIGNED_BYTE] = GL_UNSIGNED_BYTE;
    return M;
}

template<size_t T>
constexpr std::array<int, T> generateBufferTypeMapper()
{
    std::array<int, T> M{ 0 };
    M[(int)BufferType::ARRAY_BUFFER] = GL_ARRAY_BUFFER;
    M[(int)BufferType::INDEX_BUFFER] = GL_ELEMENT_ARRAY_BUFFER;
    return M;
}

template<size_t T>
constexpr std::array<int, T> generatePrimitiveTypeMapper()
{
    std::array<int, T> M{ 0 };
    M[(int)PrimitiveType::TRIANGLE_LIST] = GL_TRIANGLES;
    M[(int)PrimitiveType::TRIANGLE_STRIP] = GL_TRIANGLE_STRIP;
    M[(int)PrimitiveType::LINE_LIST] = GL_LINES;
    M[(int)PrimitiveType::LINE_STRIP] = GL_LINE_STRIP;
    M[(int)PrimitiveType::POINTS] = GL_POINTS;
    return M;
}


template<size_t T>
constexpr std::array<int, T> generateDepthTestingFunctionMapper()
{
    std::array<int, T> M{ 0 };
    M[(int)DepthTestingFunction::ALWAYS] = GL_ALWAYS;
    M[(int)DepthTestingFunction::NEVER] = GL_NEVER;
    M[(int)DepthTestingFunction::LESS] = GL_LESS;
    M[(int)DepthTestingFunction::EQUAL] = GL_EQUAL;
    M[(int)DepthTestingFunction::LEQUAL] = GL_LEQUAL;
    M[(int)DepthTestingFunction::GREATER] = GL_GREATER;
    M[(int)DepthTestingFunction::GEQUAL] = GL_GEQUAL;
    M[(int)DepthTestingFunction::NEQUAL] = GL_NOTEQUAL;
    return M;
}

static constexpr auto TextureWarpMethodMapper = generateTextureWarpMethod<(size_t)TextureWarpMethod::__COUNT>();
static constexpr auto TextureSampleMethodMapper = generateTextureSampleMethod<(size_t)TextureSampleMethod::__COUNT>();
static constexpr auto PixelFormatMapper = generatePixelFormat<(size_t)PixelFormat::__COUNT>();
static constexpr auto ShaderTypeMapper = generateShaderTypeMapper<(size_t)ShaderType::__COUNT>();
static constexpr auto DataTypeToGLMapper = generateDataTypeMapper<(size_t)EngineDataType::__COUNT>();
static constexpr auto BufferTypeMapper = generateBufferTypeMapper<(size_t)BufferType::__COUNT>();
static constexpr auto DrawPrimitivesTypeMapper = generatePrimitiveTypeMapper<(size_t)PrimitiveType::__COUNT>();
static constexpr auto DepthTestingFunctionMapper = generateDepthTestingFunctionMapper<(size_t)DepthTestingFunction::__COUNT>();

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char* message,
                            const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    SeverityLevel level = SeverityLevel::Info;

    std::string str;
    str.append("---------------\n");
    str.append(string_format("Debug message (%d): %s\n", id, message));

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             str.append("Source: API"); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   str.append("Source: Window System"); break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: str.append("Source: Shader Compiler"); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     str.append("Source: Third Party"); break;
    case GL_DEBUG_SOURCE_APPLICATION:     str.append("Source: Application"); break;
    case GL_DEBUG_SOURCE_OTHER:           str.append("Source: Other"); break;
    }

    str.append("\n");

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
    {
        level = SeverityLevel::Error;
        str.append("Type: Error"); 
        break;
    }
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    {
        level = SeverityLevel::Warning;
        str.append("Type: Deprecated Behaviour"); 
        break;
    }
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    {
        level = SeverityLevel::Warning;
        str.append("Type: Undefined Behaviour"); 
        break;
    }
    case GL_DEBUG_TYPE_PORTABILITY:         str.append("Type: Portability"); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         str.append("Type: Performance"); break;
    case GL_DEBUG_TYPE_MARKER:              str.append("Type: Marker"); break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          str.append("Type: Push Group"); break;
    case GL_DEBUG_TYPE_POP_GROUP:           str.append("Type: Pop Group"); break;
    case GL_DEBUG_TYPE_OTHER:               str.append("Type: Other"); break;
    } 

    str.append("\n");

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         str.append("Severity: high"); break;
    case GL_DEBUG_SEVERITY_MEDIUM:       str.append("Severity: medium"); break;
    case GL_DEBUG_SEVERITY_LOW:          str.append("Severity: low"); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: str.append("Severity: notification"); break;
    }

    str.append("\n");
    
    auto logger = Engine::GetInstance()->GetLogger();
    logger->Log(level, "%s", str.c_str());

    if (level == SeverityLevel::Error)
    {
        throw std::exception("OpenGL error triggered!");
    }
}


OpenGLProvider::OpenGLProvider(const EngineSettings& settings)
{
	// Initialize GLAD and configs
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::exception("Failed to load glad!");
	}
#ifdef _DEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        Engine::GetInstance()->GetLogger()->Log(SeverityLevel::Info, "Debug context started");
    }
#endif

	_graphicsDevice = std::make_shared<OpenGLGraphicsDevice>(settings);
    _graphicsResourceManager = std::make_shared<OpenGLGraphicsResourceManager>(settings);
}

int OpenGLProvider::MapTextureWarpMethod(TextureWarpMethod warpMethod)
{
    return TextureWarpMethodMapper[(int)warpMethod];
}

std::array<int, 2> OpenGLProvider::MapTextureSampleMethod(TextureSampleMethod sampleMethod)
{
    return TextureSampleMethodMapper[(int)sampleMethod];
}

int OpenGLProvider::MapPixelFormat(PixelFormat format)
{
    return PixelFormatMapper[(int)format];
}

int OpenGLProvider::MapShaderType(ShaderType type)
{
    return ShaderTypeMapper[(int)type];
}

int OpenGLProvider::MapDataType(EngineDataType type)
{
    return DataTypeToGLMapper[(int)type];
}

int OpenGLProvider::MapBufferType(BufferType type)
{
    return BufferTypeMapper[(int)type];
}

int OpenGLProvider::MapDrawPrimitivesType(PrimitiveType type)
{
    return DrawPrimitivesTypeMapper[(int)type];
}

int OpenGLProvider::MapDepthTestingFunctionType(DepthTestingFunction func)
{
    return DepthTestingFunctionMapper[(int)func];
}

TRV2_NAMESPACE_END