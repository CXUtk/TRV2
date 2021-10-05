#include "GLFWProvider.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Platform/GameWindow/GLFWGameWindow.h>
#include <Platform/FileSystem/WindowsFileSystem.h>

#include <Core/Utils/GameTimer.h>
#include <Core/Structures/EngineSettings.h>

TRV2_NAMESPACE_BEGIN
GLFWProvider::GLFWProvider(const EngineSettings& settings)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, settings.IsWindowResizable() ? GLFW_TRUE : GLFW_FALSE);

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	_gameWindow = std::make_unique<GLFWGameWindow>(settings);
    _fileSystem = std::make_unique<WindowsFileSystem>();
}

GLFWProvider::~GLFWProvider()
{}
TRV2_NAMESPACE_END