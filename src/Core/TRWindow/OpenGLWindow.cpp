#include "OpenGLWindow.h"
#include <Configs/ClientConfig.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

OpenGLWindow::OpenGLWindow() : _window(nullptr)
{
}

OpenGLWindow::~OpenGLWindow()
{
    if (_window) {
        glfwDestroyWindow(_window);
    }
    _window = nullptr;
}

void OpenGLWindow::Initialize(const ClientConfig* config)
{
    _window = glfwCreateWindow(config->GetClientWidth(), config->GetClientHeight(),
        config->GetClientTitle().c_str(), nullptr, nullptr);
    if (!_window) {
        throw std::exception("Failed to create window");
    }
    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
}

void OpenGLWindow::BeginFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool OpenGLWindow::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}

void OpenGLWindow::SwapBuffers()
{
    glfwSwapBuffers(_window);
}

void OpenGLWindow::PollEvents()
{
    glfwPollEvents();
}
