#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Core/Core_Interfaces.h>
#include <Core/InputController/GLFWInputController.h>

TRV2_NAMESPACE_BEGIN
class GLFWGameWindow : public IGameWindow
{
public:
	GLFWGameWindow(const EngineSettings* config);
	~GLFWGameWindow() override;

	void BeginFrame() override;
	void EndFrame() override;

	bool ShouldClose() const override;
	void PollEvents() override;

	const IInputController* GetInputController() const override { return trv2::cptr(_inputController); }

	glm::ivec2 GetWindowSize() const override;
private:
	GLFWwindow* _window;
	std::shared_ptr<GLFWInputController> _inputController;
};
TRV2_NAMESPACE_END