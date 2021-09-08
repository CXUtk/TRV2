﻿#include "GLFWGameWindow.h"
#include <Configs/EngineSettings.h>

TRV2_NAMESPACE_BEGIN
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWGameWindow::GLFWGameWindow(const EngineSettings& config)
{
    _window = glfwCreateWindow(config.GetWindowWidth(), config.GetWindowHeight(),
        config.GetWindowTitle(), nullptr, nullptr);
    if (!_window)
    {
        throw std::exception("Failed to create window");
    }
    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
}

GLFWGameWindow::~GLFWGameWindow()
{
    if (_window) {
        glfwDestroyWindow(_window);
    }
    _window = nullptr;
    glfwTerminate();
}


void GLFWGameWindow::BeginFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFWGameWindow::EndFrame()
{
    glfwSwapBuffers(_window);
}

bool GLFWGameWindow::ShouldClose() const
{
    return glfwWindowShouldClose(_window);
}

void GLFWGameWindow::PollEvents()
{
    glfwPollEvents();
}
glm::ivec2 GLFWGameWindow::GetWindowSize() const
{
    int x, y;
    glfwGetWindowSize(_window, &x, &y);
    return glm::ivec2(x, y);
}
TRV2_NAMESPACE_END