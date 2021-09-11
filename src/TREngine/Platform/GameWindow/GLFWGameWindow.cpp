#include "GLFWGameWindow.h"
#include <vector>
#include <array>
#include <Core/Structures/EngineSettings.h>
#include <Core/Gamplay/InputController.h>
#include <Engine.h>
#include <Core/Utils/Logging/Logger.h>

TRV2_NAMESPACE_BEGIN


template<int T>
std::array<std::vector<int>, T> generateKeyCodeMapper()
{
    std::array<std::vector<int>, T> M{};
    M[(int)KeyCode::TRV2_W_KEY] = { GLFW_KEY_W };
    M[(int)KeyCode::TRV2_S_KEY] = { GLFW_KEY_S };
    M[(int)KeyCode::TRV2_A_KEY] = { GLFW_KEY_A };
    M[(int)KeyCode::TRV2_D_KEY] = { GLFW_KEY_D };

    M[(int)KeyCode::TRV2_ENTER_KEY] = { GLFW_KEY_ENTER };
    M[(int)KeyCode::TRV2_CTRL_KEY] = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL };
    M[(int)KeyCode::TRV2_ALT_KEY] = { GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT };

    M[(int)KeyCode::TRV2_0_KEY] = { GLFW_KEY_0 };
    M[(int)KeyCode::TRV2_1_KEY] = { GLFW_KEY_1 };
    M[(int)KeyCode::TRV2_2_KEY] = { GLFW_KEY_2 };
    M[(int)KeyCode::TRV2_3_KEY] = { GLFW_KEY_3 };
    M[(int)KeyCode::TRV2_4_KEY] = { GLFW_KEY_4 };
    M[(int)KeyCode::TRV2_5_KEY] = { GLFW_KEY_5 };
    M[(int)KeyCode::TRV2_6_KEY] = { GLFW_KEY_6 };
    M[(int)KeyCode::TRV2_7_KEY] = { GLFW_KEY_7 };
    M[(int)KeyCode::TRV2_8_KEY] = { GLFW_KEY_8 };
    M[(int)KeyCode::TRV2_9_KEY] = { GLFW_KEY_9 };

    M[(int)KeyCode::TRV2_PLUS_KEY] = { GLFW_KEY_EQUAL };
    M[(int)KeyCode::TRV2_MINUS_KEY] = { GLFW_KEY_MINUS };
    M[(int)KeyCode::TRV2_TILDE_KEY] = { GLFW_KEY_GRAVE_ACCENT };
    return M;
}

template<int T>
std::array<std::vector<int>, T> generateMouseButtonCodeMapper()
{
    std::array<std::vector<int>, T> M{ };

    M[(int)MouseButtonCode::LEFT_BUTTON] = { GLFW_MOUSE_BUTTON_LEFT };
    M[(int)MouseButtonCode::RIGHT_BUTTON] = { GLFW_MOUSE_BUTTON_RIGHT };
    M[(int)MouseButtonCode::MIDDLE_BUTTON] = { GLFW_MOUSE_BUTTON_MIDDLE };
    return M;
}
static const auto keyCodeMap = generateKeyCodeMapper<(int)KeyCode::__COUNT>();
static const auto mouseButtonCodeMap = generateMouseButtonCodeMapper<(int)MouseButtonCode::__COUNT>();

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    auto gameWindow = (GLFWGameWindow*)glfwGetWindowUserPointer(window);
    gameWindow->Resize(glm::ivec2(width, height));

    glViewport(0, 0, width, height);
}

static void mouseScrollCallbackFunction(GLFWwindow* window, double xoffset, double yoffset)
{
    auto inputController = Engine::GetInstance()->GetInputController();
}

static void keyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto inputController = Engine::GetInstance()->GetInputController();
}

static void mouseButtonCallbackFunction(GLFWwindow* window, int button, int action, int mods)
{
    auto inputController = Engine::GetInstance()->GetInputController();
    if (action == GLFW_PRESS)
    {
        Engine::GetInstance()->GetLogger()->LogInfo("Mouse button press");
    }
}

GLFWGameWindow::GLFWGameWindow(const EngineSettings& settings)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, settings.IsWindowResizable() ? GLFW_TRUE : GLFW_FALSE);

    _windowSize = glm::ivec2(settings.GetWindowWidth(), settings.GetWindowHeight());

    _window = glfwCreateWindow(_windowSize.x, _windowSize.y,
        settings.GetWindowTitle(), nullptr, nullptr);
    if (!_window)
    {
        throw std::exception("Failed to create window");
    }

    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetScrollCallback(_window, mouseScrollCallbackFunction);
    glfwSetMouseButtonCallback(_window, mouseButtonCallbackFunction);
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
TRV2_NAMESPACE_END