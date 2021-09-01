#pragma once

#include <bitset>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <TREngine_Interfaces.h>
#include <Core/Interfaces/IInputController.h>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
class OpenGLInputController : public IInputController
{
public:
	OpenGLInputController(GLFWwindow* window);
	~OpenGLInputController();

	void ClearInput() override;
    void UpdateInput() override;

	void ScrollWheel(glm::vec2 dir);


	glm::vec2 GetScrollValue() const override { return _scrollWheel; }

	bool IsKeyJustPressed(int key) const override { return !_wasKeysDown[key] && _curKeysDown[key]; }
	bool IsKeyDowned(int key) const override { return _curKeysDown[key]; }
	bool IsKeyJustReleased(int key) const override { return _wasKeysDown[key] && !_curKeysDown[key]; }

	bool IsMouseClicked(int mouse) const override { return !_wasMouseButtonDown[mouse] && _curMouseButtonDown[mouse]; }
	bool IsMouseDowned(int mouse) const override { return _curMouseButtonDown[mouse]; }
	bool IsMouseReleased(int mouse) const override { return _wasMouseButtonDown[mouse] && !_curMouseButtonDown[mouse]; }

private:
	GLFWwindow* _window;

	// Keys
	std::bitset<512> _curKeysDown;
	std::bitset<512> _wasKeysDown;

	// Mouse wheel
	glm::vec2 _scrollWheel;

	// Mouse button
	std::bitset<8> _curMouseButtonDown;
	std::bitset<8> _wasMouseButtonDown;
};
TRV2_NAMESPACE_END