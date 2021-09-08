#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <TREngine_Interfaces.hpp>

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