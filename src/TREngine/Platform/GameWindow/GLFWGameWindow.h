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

	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual bool ShouldClose() const override;
	virtual void PollEvents() override;


	glm::ivec2 GetMousePos() const override;
	glm::ivec2 GetWindowSize() const override { return _windowSize; }

	virtual void AppendOnResizeEvent(Event<glm::ivec2>::func_type eventHandler) override
	{
		_eventOnWindowResize += eventHandler;
	}

	void Resize(glm::ivec2 newSize)
	{
		_eventOnWindowResize.Invoke(newSize);
	}

private:
	GLFWwindow* _window;
	glm::ivec2 _windowSize;

	Event<glm::ivec2> _eventOnWindowResize;
};
TRV2_NAMESPACE_END