#include "OpenGLGraphicsDevice.h"


#include <exception>
#include <TREngine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Configs/EngineSettings.h>
#include <Assets/AssetsManager.h>
#include <Graphics/Renderers/OpenGLSpriteRenderer.h>

TRV2_NAMESPACE_BEGIN
OpenGLGraphicsDevice::OpenGLGraphicsDevice()
{
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
    glfwTerminate();
}

void OpenGLGraphicsDevice::Initialize(const EngineSettings& clientConfig)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

std::shared_ptr<ISpriteRenderer> OpenGLGraphicsDevice::CreateSpriteRenderer() const
{
	return std::make_shared<OpenGLSpriteRenderer>(this);
}
TRV2_NAMESPACE_END