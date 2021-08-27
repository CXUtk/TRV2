#include "OpenGLGraphicsDevice.h"
#include <Config/ClientConfig.h>
#include <glm/glm.hpp>
#include <exception>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}


OpenGLGraphicsDevice::OpenGLGraphicsDevice(const std::shared_ptr<ClientConfig>& clientConfig)
{
    _window = nullptr;
    _clientConfig = clientConfig;
}

void OpenGLGraphicsDevice::Initialize()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(_clientConfig->GetClientWidth(), _clientConfig->GetClientHeight(),
        _clientConfig->GetClientTitle().c_str(), nullptr, nullptr);
    if (!_window) {
        // fprintf(stderr, "Failed to create window\n");
        throw std::exception("Failed to create window");
    }

    glfwMakeContextCurrent(_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        // fprintf(stderr, "Failed to load glad!\n");
        throw std::exception("Failed to load glad!");
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void OpenGLGraphicsDevice::Loop()
{
    double minElapsedTime = 1.0 / _clientConfig->GetFPSCap();
    double prevTimestamp = glfwGetTime();
    while (!glfwWindowShouldClose(_window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(_window);

        while (glfwGetTime() - prevTimestamp < minElapsedTime) {
            glfwPollEvents();
        }
        prevTimestamp = glfwGetTime();
    }
}

OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
{
    if (_window) {
        glfwDestroyWindow(_window);
    }
    glfwTerminate();
}
