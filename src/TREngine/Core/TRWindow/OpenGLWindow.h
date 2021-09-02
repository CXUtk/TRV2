#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Interfaces/ITRWindow.h>
#include <Core/Interfaces/IInputController.h>
#include <Core/InputController/OpenGLInputController.h>
TRV2_NAMESPACE_BEGIN
class OpenGLWindow : public ITRWindow
{
public:
	OpenGLWindow();
	~OpenGLWindow();

	void Initialize(const EngineSettings& config) override;

	void BeginFrame() override;
	void EndFrame() override;

	bool ShouldClose() const override;
	void PollEvents() override;

	const IInputController& GetInputController() const override { return trv2::cref(_inputController); }

	glm::ivec2 GetWindowSize() const override;
private:
	GLFWwindow* _window;
	std::shared_ptr<OpenGLInputController> _inputController;
};
TRV2_NAMESPACE_END