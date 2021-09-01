#include "TROpenGLAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Core/TRWindow/OpenGLWindow.h>

TRV2_NAMESPACE_BEGIN
TROpenGLAPIGenerator::TROpenGLAPIGenerator()
{
}

TROpenGLAPIGenerator::~TROpenGLAPIGenerator()
{
}

void TROpenGLAPIGenerator::Initialize(const EngineSettings& config)
{
	// Create upper layers
	_graphicsDevice = std::make_shared<OpenGLGraphicsDevice>();
	_graphicsDevice->Initialize(config);

	_gameWindow = std::make_shared<OpenGLWindow>();
	_gameWindow->Initialize(config);

	_graphicsAPIUtils = std::make_shared<TROpenGLAPIUtils>();


	// Initialize GLAD and configs
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::exception("Failed to load glad!");
	}
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

TROpenGLAPIUtils::TROpenGLAPIUtils()
{
}

double TROpenGLAPIUtils::GetTime()
{
	return glfwGetTime();
}
TRV2_NAMESPACE_END