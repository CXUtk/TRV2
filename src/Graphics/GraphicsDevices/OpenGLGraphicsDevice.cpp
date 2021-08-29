#include "OpenGLGraphicsDevice.h"


#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Configs/ClientConfig.h>

OpenGLGraphicsDevice::OpenGLGraphicsDevice()
{
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
    glfwTerminate();
}

void OpenGLGraphicsDevice::Initialize(const ClientConfig* clientConfig)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}
