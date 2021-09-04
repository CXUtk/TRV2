#include "GLFWInitializer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Configs/EngineSettings.h>

TRV2_NAMESPACE_BEGIN
trv2::GLFWInitializer::GLFWInitializer()
{}

trv2::GLFWInitializer::~GLFWInitializer()
{}

void GLFWInitializer::Initialize(const EngineSettings& config)
{
	// Create upper layers
	_graphicsDevice = std::make_shared<OpenGLGraphicsDevice>();
	_graphicsDevice->Initialize(config);

	_gameWindow = std::make_shared<GLFWGameWindow>();
	_gameWindow->Initialize(config);

	_gameTimer = std::make_shared<GLFWGameTimer>();

	// Initialize GLAD and configs
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::exception("Failed to load glad!");
	}
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}
TRV2_NAMESPACE_END