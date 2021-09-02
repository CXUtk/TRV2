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

	bool IsKeyJustPressed(TRV2KeyCode key) const override { return !_wasKeysDown[(int)key] && _curKeysDown[(int)key]; }
	bool IsKeyDowned(TRV2KeyCode key) const override { return _curKeysDown[(int)key]; }
	bool IsKeyJustReleased(TRV2KeyCode key) const override { return _wasKeysDown[(int)key] && !_curKeysDown[(int)key]; }

	bool IsMouseClicked(TRV2MouseButtonCode mouse) const override { return !_wasMouseButtonDown[(int)mouse] && _curMouseButtonDown[(int)mouse]; }
	bool IsMouseDowned(TRV2MouseButtonCode mouse) const override { return _curMouseButtonDown[(int)mouse]; }
	bool IsMouseReleased(TRV2MouseButtonCode mouse) const override { return _wasMouseButtonDown[(int)mouse] && !_curMouseButtonDown[(int)mouse]; }

	glm::vec2 GetMousePos() const override;

private:
	GLFWwindow* _window;

	// Keys
	std::bitset<TRV2_Input_KeyCode_SIZE> _curKeysDown;
	std::bitset<TRV2_Input_KeyCode_SIZE> _wasKeysDown;

	// Mouse wheel
	glm::vec2 _scrollWheel;

	// Mouse button
	std::bitset<TRV2_Input_MouseButtonCode_SIZE> _curMouseButtonDown;
	std::bitset<TRV2_Input_MouseButtonCode_SIZE> _wasMouseButtonDown;


	void initializeCodeMapping();
};
TRV2_NAMESPACE_END