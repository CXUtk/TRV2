#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Platform/InputController/GLFWInputController.h>

TRV2_NAMESPACE_BEGIN
class GLFWGameWindow
{
public:
	GLFWGameWindow(const EngineSettings& config);
	~GLFWGameWindow();

	void BeginFrame();
	void EndFrame();

	bool ShouldClose() const;
	void PollEvents();


	glm::ivec2 GetWindowSize() const;
	GLFWwindow* GetWindowHandle() const { return _window; }
private:
	GLFWwindow* _window;
};
TRV2_NAMESPACE_END