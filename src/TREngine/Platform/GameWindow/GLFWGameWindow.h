#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Core.h>
#include <Platform/Platform_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class GLFWGameWindow : public IGameWindow
{
public:
	GLFWGameWindow(const EngineSettings& settings);
	~GLFWGameWindow();

	void BeginFrame();
	void EndFrame();

	bool ShouldClose() const;
	void PollEvents();


	glm::ivec2 GetMousePos() const override;
	glm::ivec2 GetWindowSize() const override { return _windowSize; }

	void Resize(glm::ivec2 newSize) { _windowSize = newSize; }

private:
	GLFWwindow* _window;
	glm::ivec2 _windowSize;
};
TRV2_NAMESPACE_END