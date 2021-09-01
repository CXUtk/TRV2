#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Interfaces/ITRWindow.h>
TRV2_NAMESPACE_BEGIN
class OpenGLWindow :public ITRWindow
{
public:
	OpenGLWindow();
	~OpenGLWindow();

	void Initialize(const EngineSettings& config) override;
	void BeginFrame() override;
	bool ShouldClose() const override;
	void SwapBuffers() override;
	void PollEvents() override;
private:
	GLFWwindow* _window;
};
TRV2_NAMESPACE_END