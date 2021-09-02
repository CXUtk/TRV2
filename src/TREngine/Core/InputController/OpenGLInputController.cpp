#include "OpenGLInputController.h"

TRV2_NAMESPACE_BEGIN
OpenGLInputController::OpenGLInputController(GLFWwindow* window) : _window(window), _scrollWheel(glm::vec2(0))
{

    glfwSetWindowUserPointer(window, this);
    auto mouseScrollCallbackFunction = [](GLFWwindow* window, double xoffset, double yoffset) {
        auto controller = (OpenGLInputController*)glfwGetWindowUserPointer(window);
        controller->ScrollWheel(glm::vec2(xoffset, yoffset));
    };
    glfwSetScrollCallback(_window, mouseScrollCallbackFunction);
}

OpenGLInputController::~OpenGLInputController()
{
}

void OpenGLInputController::ClearInput()
{
    _scrollWheel = glm::vec2(0);

    _wasMouseButtonDown = _curMouseButtonDown;
    _curMouseButtonDown.reset();

    _wasKeysDown = _curKeysDown;
    _curKeysDown.reset();
}

void OpenGLInputController::UpdateInput()
{
    _curMouseButtonDown[0] = (glfwGetMouseButton(_window, 0) == GLFW_PRESS);
    for (int i = 0; i < 512; i++) {
        _curKeysDown[i] = (glfwGetKey(_window, i) == GLFW_PRESS);
    }
}

void OpenGLInputController::ScrollWheel(glm::vec2 dir)
{
    _scrollWheel += dir;
}
glm::vec2 OpenGLInputController::GetMousePos() const
{
    double x, y;
    glfwGetCursorPos(_window, &x, &y);

    int w, h;
    glfwGetWindowSize(_window, &w, &h);
    return glm::vec2(x, h - y - 1);
}
TRV2_NAMESPACE_END