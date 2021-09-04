#pragma once
#include <TREngine_Interfaces.h>
#include <Core/Core_Interfaces.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

TRV2_NAMESPACE_BEGIN
class GLFWGameTimer : public IGameTimer
{
public:
	GLFWGameTimer() {}
	~GLFWGameTimer() override {}

	double GetCurrentTime() override
	{
		return glfwGetTime();
	}
};
TRV2_NAMESPACE_END