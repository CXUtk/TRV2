#include "GLFWInputController.h"
#include <Core/GameWindow/GLFWGameWindow.h>
#include <vector>

TRV2_NAMESPACE_BEGIN
static constexpr int GLFW_KEY_SIZE = 512;
static std::vector<int> keyCodeMap[TRV2_Input_KeyCode_SIZE];
static std::vector<int> mouseButtonCodeMap[TRV2_Input_MouseButtonCode_SIZE];

void GLFWInputController::initializeCodeMapping()
{
    keyCodeMap[(int)TRV2KeyCode::TRV2_W_KEY] = { GLFW_KEY_W };
    keyCodeMap[(int)TRV2KeyCode::TRV2_S_KEY] = { GLFW_KEY_S };
    keyCodeMap[(int)TRV2KeyCode::TRV2_A_KEY] = { GLFW_KEY_A };
    keyCodeMap[(int)TRV2KeyCode::TRV2_D_KEY] = { GLFW_KEY_D };

    keyCodeMap[(int)TRV2KeyCode::TRV2_ENTER_KEY] = { GLFW_KEY_ENTER };
    keyCodeMap[(int)TRV2KeyCode::TRV2_CTRL_KEY] = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL };
    keyCodeMap[(int)TRV2KeyCode::TRV2_ALT_KEY] = { GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT };

    keyCodeMap[(int)TRV2KeyCode::TRV2_0_KEY] = { GLFW_KEY_0 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_1_KEY] = { GLFW_KEY_1 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_2_KEY] = { GLFW_KEY_2 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_3_KEY] = { GLFW_KEY_3 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_4_KEY] = { GLFW_KEY_4 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_5_KEY] = { GLFW_KEY_5 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_6_KEY] = { GLFW_KEY_6 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_7_KEY] = { GLFW_KEY_7 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_8_KEY] = { GLFW_KEY_8 };
    keyCodeMap[(int)TRV2KeyCode::TRV2_9_KEY] = { GLFW_KEY_9 };

    keyCodeMap[(int)TRV2KeyCode::TRV2_PLUS_KEY] = { GLFW_KEY_EQUAL };
    keyCodeMap[(int)TRV2KeyCode::TRV2_MINUS_KEY] = { GLFW_KEY_MINUS };
    keyCodeMap[(int)TRV2KeyCode::TRV2_TILDE_KEY] = { GLFW_KEY_GRAVE_ACCENT };

    mouseButtonCodeMap[(int)TRV2MouseButtonCode::LEFT_BUTTON] = { GLFW_MOUSE_BUTTON_LEFT };
    mouseButtonCodeMap[(int)TRV2MouseButtonCode::RIGHT_BUTTON] = { GLFW_MOUSE_BUTTON_RIGHT };
    mouseButtonCodeMap[(int)TRV2MouseButtonCode::MIDDLE_BUTTON] = { GLFW_MOUSE_BUTTON_MIDDLE };
}

GLFWInputController::GLFWInputController(GLFWwindow* window) : _window(window), _scrollWheel(glm::vec2(0))
{
    initializeCodeMapping();
    auto mouseScrollCallbackFunction = [](GLFWwindow* window, double xoffset, double yoffset) {
        auto gameWindow = (GLFWGameWindow*)glfwGetWindowUserPointer(window);
        ((GLFWInputController&)gameWindow->GetInputController()).ScrollWheel(glm::vec2(xoffset, yoffset));
    };
    glfwSetScrollCallback(_window, mouseScrollCallbackFunction);
}

GLFWInputController::~GLFWInputController()
{
}

void GLFWInputController::ClearInput()
{
    _scrollWheel = glm::vec2(0);

    _wasMouseButtonDown = _curMouseButtonDown;
    _curMouseButtonDown.reset();

    _wasKeysDown = _curKeysDown;
    _curKeysDown.reset();
}

void GLFWInputController::UpdateInput()
{
    for (int i = 0; i < TRV2_Input_MouseButtonCode_SIZE; i++) {
        for (auto code : mouseButtonCodeMap[i]) {
            _curMouseButtonDown[i] = (glfwGetMouseButton(_window, code) == GLFW_PRESS);
        }
    }

    for (int i = 0; i < TRV2_Input_KeyCode_SIZE; i++) {
        for (auto code : keyCodeMap[i]) {
            _curKeysDown[i] = (glfwGetKey(_window, code) == GLFW_PRESS);
        }
    }
}

void GLFWInputController::ScrollWheel(glm::vec2 dir)
{
    _scrollWheel += dir;
}
glm::vec2 GLFWInputController::GetMousePos() const
{
    double x, y;
    glfwGetCursorPos(_window, &x, &y);

    int w, h;
    glfwGetWindowSize(_window, &w, &h);
    return glm::vec2(x, h - y - 1);
}

TRV2_NAMESPACE_END