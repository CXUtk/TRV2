#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLGraphicsAPI.hpp"
#include <Configs/EngineSettings.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>


TRV2_NAMESPACE_BEGIN
_OpenGLAPI::_OpenGLAPI(const EngineSettings& settings)
{
	// Initialize GLAD and configs
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::exception("Failed to load glad!");
	}
	_graphicsDevice = std::make_shared<_GraphicsDevice_Type>(settings);
}

TRV2_NAMESPACE_END