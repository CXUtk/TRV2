#include "OpenGLTRGameGraphicsAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Core/TRWindow/OpenGLWindow.h>

OpenGLTRGameGraphicsAPIGenerator::OpenGLTRGameGraphicsAPIGenerator(TREngine* engine) : _engine(engine)
{
}

OpenGLTRGameGraphicsAPIGenerator::~OpenGLTRGameGraphicsAPIGenerator()
{
}

void OpenGLTRGameGraphicsAPIGenerator::Initialize(const ClientConfig* config)
{
	// Create upper layers
	_graphicsDevice = std::make_shared<OpenGLGraphicsDevice>(_engine);
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
