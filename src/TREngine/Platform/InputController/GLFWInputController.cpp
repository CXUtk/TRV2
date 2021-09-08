#include "GLFWInputController.h"
#include <array>
#include <vector>

TRV2_NAMESPACE_BEGIN
static constexpr int GLFW_KEY_SIZE = 512;



template<int T>
std::array<std::vector<int>, T> generateKeyCodeMapper()
{
    std::array<std::vector<int>, T> M{};
    M[(int)TRV2KeyCode::TRV2_W_KEY] = { GLFW_KEY_W };
    M[(int)TRV2KeyCode::TRV2_S_KEY] = { GLFW_KEY_S };
    M[(int)TRV2KeyCode::TRV2_A_KEY] = { GLFW_KEY_A };
    M[(int)TRV2KeyCode::TRV2_D_KEY] = { GLFW_KEY_D };

    M[(int)TRV2KeyCode::TRV2_ENTER_KEY] = { GLFW_KEY_ENTER };
    M[(int)TRV2KeyCode::TRV2_CTRL_KEY] = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL };
    M[(int)TRV2KeyCode::TRV2_ALT_KEY] = { GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT };

    M[(int)TRV2KeyCode::TRV2_0_KEY] = { GLFW_KEY_0 };
    M[(int)TRV2KeyCode::TRV2_1_KEY] = { GLFW_KEY_1 };
    M[(int)TRV2KeyCode::TRV2_2_KEY] = { GLFW_KEY_2 };
    M[(int)TRV2KeyCode::TRV2_3_KEY] = { GLFW_KEY_3 };
    M[(int)TRV2KeyCode::TRV2_4_KEY] = { GLFW_KEY_4 };
    M[(int)TRV2KeyCode::TRV2_5_KEY] = { GLFW_KEY_5 };
    M[(int)TRV2KeyCode::TRV2_6_KEY] = { GLFW_KEY_6 };
    M[(int)TRV2KeyCode::TRV2_7_KEY] = { GLFW_KEY_7 };
    M[(int)TRV2KeyCode::TRV2_8_KEY] = { GLFW_KEY_8 };
    M[(int)TRV2KeyCode::TRV2_9_KEY] = { GLFW_KEY_9 };

    M[(int)TRV2KeyCode::TRV2_PLUS_KEY] = { GLFW_KEY_EQUAL };
    M[(int)TRV2KeyCode::TRV2_MINUS_KEY] = { GLFW_KEY_MINUS };
    M[(int)TRV2KeyCode::TRV2_TILDE_KEY] = { GLFW_KEY_GRAVE_ACCENT };
    return M;
}

template<int T>
std::array<std::vector<int>, T> generateMouseButtonCodeMapper()
{
    std::array<std::vector<int>, T> M{ };

    M[(int)TRV2MouseButtonCode::LEFT_BUTTON] = { GLFW_MOUSE_BUTTON_LEFT };
    M[(int)TRV2MouseButtonCode::RIGHT_BUTTON] = { GLFW_MOUSE_BUTTON_RIGHT };
    M[(int)TRV2MouseButtonCode::MIDDLE_BUTTON] = { GLFW_MOUSE_BUTTON_MIDDLE };
    return M;
}
static const auto keyCodeMap = generateKeyCodeMapper<(int)TRV2KeyCode::__COUNT>();
static const auto mouseButtonCodeMap = generateMouseButtonCodeMapper<(int)TRV2MouseButtonCode::__COUNT>();

void GLFWInputController::initializeCodeMapping()
{
}

GLFWInputController::GLFWInputController(GLFWwindow* window, const EngineSettings& settings)
    : _window(window), _scrollWheel(glm::vec2(0))
{
    glfwSetWindowUserPointer(_window, this);
    initializeCodeMapping();
    auto mouseScrollCallbackFunction = [](GLFWwindow* rawWindow, double xoffset, double yoffset) {
        auto controller = (GLFWInputController*)glfwGetWindowUserPointer(rawWindow);
        controller->ScrollWheel(glm::vec2(xoffset, yoffset));
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
    for (int i = 0; i < (int)TRV2MouseButtonCode::__COUNT; i++) {
        for (auto code : mouseButtonCodeMap[i]) {
            _curMouseButtonDown[i] = (glfwGetMouseButton(_window, code) == GLFW_PRESS);
        }
    }

    for (int i = 0; i < (int)TRV2KeyCode::__COUNT; i++) {
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