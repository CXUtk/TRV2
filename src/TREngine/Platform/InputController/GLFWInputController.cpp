//#include "GLFWInputController.hpp"
//
//#include <array>
//#include <vector>
//
//TRV2_NAMESPACE_BEGIN
//static constexpr int GLFW_KEY_SIZE = 512;
//
//
//void GLFWInputController::initializeCodeMapping()
//{
//}
//
//GLFWInputController::GLFWInputController(GLFWwindow* window, const EngineSettings& settings)
//    : _window(window), _scrollWheel(glm::vec2(0))
//{
//    assert(window != nullptr);
//
//    glfwSetWindowUserPointer(_window, this);
//    initializeCodeMapping();
//    auto mouseScrollCallbackFunction = [](GLFWwindow* rawWindow, double xoffset, double yoffset) {
//        auto controller = (GLFWInputController*)glfwGetWindowUserPointer(rawWindow);
//        controller->ScrollWheel(glm::vec2(xoffset, yoffset));
//    };
//    glfwSetScrollCallback(_window, mouseScrollCallbackFunction);
//}
//
//GLFWInputController::~GLFWInputController()
//{
//}
//
//void GLFWInputController::ClearInput()
//{
//    _scrollWheel = glm::vec2(0);
//
//    _wasMouseButtonDown = _curMouseButtonDown;
//    _curMouseButtonDown.reset();
//
//    _wasKeysDown = _curKeysDown;
//    _curKeysDown.reset();
//}
//
//void GLFWInputController::UpdateInput()
//{
//    for (int i = 0; i < (int)TRV2MouseButtonCode::__COUNT; i++) {
//        for (auto code : mouseButtonCodeMap[i]) {
//            _curMouseButtonDown[i] = (glfwGetMouseButton(_window, code) == GLFW_PRESS);
//        }
//    }
//
//    for (int i = 0; i < (int)TRV2KeyCode::__COUNT; i++) {
//        for (auto code : keyCodeMap[i]) {
//            _curKeysDown[i] = (glfwGetKey(_window, code) == GLFW_PRESS);
//        }
//    }
//}
//
//void GLFWInputController::ScrollWheel(glm::vec2 dir)
//{
//    _scrollWheel += dir;
//}
//glm::vec2 GLFWInputController::GetMousePos() const
//{
//    double x, y;
//    glfwGetCursorPos(_window, &x, &y);
//
//    int w, h;
//    glfwGetWindowSize(_window, &w, &h);
//    return glm::vec2(x, h - y - 1);
//}
//
//TRV2_NAMESPACE_END