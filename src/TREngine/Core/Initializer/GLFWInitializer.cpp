#include "GLFWInitializer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Configs/EngineSettings.h>

TRV2_NAMESPACE_BEGIN
trv2::GLFWInitializer::GLFWInitializer(const EngineSettings* config) : IEngineInitializer(config)
{
	// Create upper layers
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, config->IsWindowResizable() ? GLFW_TRUE : GLFW_FALSE);

	_gameWindow = std::make_shared<GLFWGameWindow>(config);

	// Initialize GLAD and configs
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::exception("Failed to load glad!");
	}
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	_graphicsDevice = std::make_shared<OpenGLGraphicsDevice>(config);

	_gameTimer = std::make_shared<GLFWGameTimer>();
}


trv2::GLFWInitializer::~GLFWInitializer()
{}

TRV2_NAMESPACE_END