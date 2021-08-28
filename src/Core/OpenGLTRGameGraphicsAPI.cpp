﻿#include "OpenGLTRGameGraphicsAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/OpenGLWindow.h>

OpenGLTRGameGraphicsAPI::OpenGLTRGameGraphicsAPI()
{
}

OpenGLTRGameGraphicsAPI::~OpenGLTRGameGraphicsAPI()
{
}

void OpenGLTRGameGraphicsAPI::Initialize(const ClientConfig* config)
{
	// Create upper layers
	_graphicsDevice = std::make_shared<OpenGLGraphicsDevice>();
	_graphicsDevice->Initialize(config);

	_gameWindow = std::make_shared<OpenGLWindow>();
	_gameWindow->Initialize(config);

	_graphicsAPIUtils = std::make_shared<OpenGLTRGameGraphicsAPIUtils>();


	// Initialize GLAD and configs
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::exception("Failed to load glad!");
	}
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

OpenGLTRGameGraphicsAPIUtils::OpenGLTRGameGraphicsAPIUtils()
{
}

double OpenGLTRGameGraphicsAPIUtils::GetTime()
{
	return glfwGetTime();
}
