#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLGraphicsAPI.hpp"
#include <Configs/EngineSettings.hpp>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.hpp>


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


static constexpr auto TextureWarpMethodMapper = generateTextureWarpMethod<(size_t)TextureWarpMethod::__COUNT>();
static constexpr auto TextureSampleMethodMapper = generateTextureSampleMethod<(size_t)TextureSampleMethod::__COUNT>();
static constexpr auto PixelFormatMapper = generatePixelFormat<(size_t)PixelFormat::__COUNT>();
static constexpr auto ShaderTypeMapper = generateShaderTypeMapper<(size_t)ShaderType::__COUNT>();


_OpenGLAPI::_OpenGLAPI(const EngineSettings& settings)
{
	// Initialize GLAD and configs
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::exception("Failed to load glad!");
	}
	_graphicsDevice = std::make_shared<_GraphicsDevice_Type>(settings);
}

int _OpenGLAPI::MapTextureWarpMethod(TextureWarpMethod warpMethod)
{
    return TextureWarpMethodMapper[(int)warpMethod];
}

std::array<int, 2> _OpenGLAPI::MapTextureSampleMethod(TextureSampleMethod sampleMethod)
{
    return TextureSampleMethodMapper[(int)sampleMethod];
}

int _OpenGLAPI::MapPixelFormat(PixelFormat format)
{
    return PixelFormatMapper[(int)format];
}

int _OpenGLAPI::MapShaderType(ShaderType type)
{
    return ShaderTypeMapper[(int)type];
}

TRV2_NAMESPACE_END