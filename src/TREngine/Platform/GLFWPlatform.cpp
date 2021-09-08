#pragma once
#include "GLFWPlatform.hpp"
#include <Configs/EngineSettings.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Platform/GameTimer/GLFWGameTimer.hpp>
#include <Platform/GameWindow/GLFWGameWindow.hpp>
#include <Platform/InputController/GLFWInputController.hpp>


TRV2_NAMESPACE_BEGIN
_GLFWPlatform::_GLFWPlatform(const EngineSettings& settings)
{
	// Create upper layers
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, settings.IsWindowResizable() ? GLFW_TRUE : GLFW_FALSE);

	_gameWindow = std::make_shared<_Window_Type>(settings);
	_inputController = std::make_shared<_InputController_Type>(_gameWindow->GetWindowHandle(), settings);
	_gameTimer = std::make_shared<_GameTimer_Type>();
}

TRV2_NAMESPACE_END