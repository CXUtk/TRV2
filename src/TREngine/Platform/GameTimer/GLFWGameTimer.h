#pragma once
#include <TREngine_Interfaces.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

TRV2_NAMESPACE_BEGIN
class GLFWGameTimer
{
public:
	GLFWGameTimer() {}
	~GLFWGameTimer() {}

	double GetCurrentTime()
	{
		return glfwGetTime();
	}
};
TRV2_NAMESPACE_END