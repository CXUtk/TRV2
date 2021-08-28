#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Interfaces/ITRWindow.h>

class OpenGLWindow :public ITRWindow
{
public:
	OpenGLWindow();
	~OpenGLWindow();

	void Initialize(const ClientConfig* config) override;
	void BeginFrame() override;
	bool ShouldClose() const override;
	void SwapBuffers() override;
	void PollEvents() override;
private:
	GLFWwindow* _window;
};