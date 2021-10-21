#include "GLFWGameWindow.h"
#include <vector>
#include <array>
#include <Engine.h>
#include <Core/Structures/EngineSettings.h>
#include <Core/Gamplay/InputController.h>
#include <Core/Utils/Logging/Logger.h>

TRV2_NAMESPACE_BEGIN

constexpr size_t MAX_KEYS = 512;
template<size_t N>
constexpr std::array<KeyCode, N> generateKeyCodeMapper()
{
    std::array<KeyCode, N> M{};
    M[GLFW_KEY_W] = KeyCode::TRV2_W_KEY;
    M[GLFW_KEY_A] = KeyCode::TRV2_A_KEY;
    M[GLFW_KEY_S] = KeyCode::TRV2_S_KEY;
    M[GLFW_KEY_D] = KeyCode::TRV2_D_KEY;
    M[GLFW_KEY_M] = KeyCode::TRV2_M_KEY;

    M[GLFW_KEY_UP] = KeyCode::TRV2_UP_KEY;
    M[GLFW_KEY_DOWN] = KeyCode::TRV2_DOWN_KEY;
    M[GLFW_KEY_LEFT] = KeyCode::TRV2_LEFT_KEY;
    M[GLFW_KEY_RIGHT] = KeyCode::TRV2_RIGHT_KEY;

    M[GLFW_KEY_ENTER] = KeyCode::TRV2_ENTER_KEY;
    M[GLFW_KEY_LEFT_CONTROL] = KeyCode::TRV2_CTRL_KEY;
    M[GLFW_KEY_RIGHT_CONTROL] = KeyCode::TRV2_CTRL_KEY;
    M[GLFW_KEY_LEFT_ALT] = KeyCode::TRV2_ALT_KEY;
    M[GLFW_KEY_RIGHT_ALT] = KeyCode::TRV2_ALT_KEY;


    M[GLFW_KEY_1] = KeyCode::TRV2_1_KEY;
    M[GLFW_KEY_2] = KeyCode::TRV2_2_KEY;
    M[GLFW_KEY_3] = KeyCode::TRV2_3_KEY;
    M[GLFW_KEY_4] = KeyCode::TRV2_4_KEY;
    M[GLFW_KEY_5] = KeyCode::TRV2_5_KEY;
    M[GLFW_KEY_6] = KeyCode::TRV2_6_KEY;
    M[GLFW_KEY_7] = KeyCode::TRV2_7_KEY;
    M[GLFW_KEY_8] = KeyCode::TRV2_8_KEY;
    M[GLFW_KEY_9] = KeyCode::TRV2_9_KEY;
    M[GLFW_KEY_0] = KeyCode::TRV2_0_KEY;


    M[GLFW_KEY_EQUAL] = KeyCode::TRV2_PLUS_KEY;
    M[GLFW_KEY_MINUS] = KeyCode::TRV2_MINUS_KEY;
    M[GLFW_KEY_GRAVE_ACCENT] = KeyCode::TRV2_TILDE_KEY;

    M[GLFW_KEY_SPACE] = KeyCode::TRV2_SPACE_KEY;
    M[GLFW_KEY_ESCAPE] = KeyCode::TRV2_ESC_KEY;

    return M;
}

template<size_t T>
constexpr std::array<MouseButtonCode, T> generateMouseButtonCodeMapper()
{
    std::array<MouseButtonCode, T> M{ };

    M[GLFW_MOUSE_BUTTON_LEFT] = MouseButtonCode::LEFT_BUTTON;
    M[GLFW_MOUSE_BUTTON_RIGHT] = MouseButtonCode::RIGHT_BUTTON;
    M[GLFW_MOUSE_BUTTON_MIDDLE] = MouseButtonCode::MIDDLE_BUTTON;
    return M;
}
static constexpr auto keyCodeMap = generateKeyCodeMapper<MAX_KEYS>();
static constexpr auto mouseButtonCodeMap = generateMouseButtonCodeMapper<MAX_KEYS>();

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    auto gameWindow = (GLFWGameWindow*)glfwGetWindowUserPointer(window);
    gameWindow->Resize(glm::ivec2(width, height));

    glViewport(0, 0, width, height);
}

static void mouseScrollCallbackFunction(GLFWwindow* window, double xoffset, double yoffset)
{
    auto inputController = Engine::GetInstance()->GetInputController();
    inputController->OnScrollWheel(glm::vec2(xoffset, yoffset));
}

static void keyCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto inputController = Engine::GetInstance()->GetInputController();
    if (key < 0 || key >= MAX_KEYS) return;
    inputController->TriggerKeyChange(keyCodeMap[key], action != GLFW_RELEASE);
}

static void mouseButtonCallbackFunction(GLFWwindow* window, int button, int action, int mods)
{
    auto inputController = Engine::GetInstance()->GetInputController();
    inputController->TriggerMouseChange(mouseButtonCodeMap[button], action == GLFW_PRESS);
}

GLFWGameWindow::GLFWGameWindow(const EngineSettings& settings)
{
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
    glfwSetKeyCallback(_window, keyCallbackFunction);

    auto& windowSize = _windowSize;
    _eventOnWindowResize += [&windowSize](glm::ivec2 newSize) {
        windowSize = newSize;
    };
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

glm::ivec2 GLFWGameWindow::GetMousePos() const
{
    double x, y;
    glfwGetCursorPos(_window, &x, &y);

    return glm::vec2(x, _windowSize.y - y - 1);
}
TRV2_NAMESPACE_END