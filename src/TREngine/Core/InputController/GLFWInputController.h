#pragma once

#include <bitset>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <TREngine_Interfaces.h>
#include <Core/Core_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class GLFWInputController : public IInputController
{
public:
	GLFWInputController(GLFWwindow* window);
	~GLFWInputController() override;

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
	std::bitset<(int)TRV2KeyCode::__COUNT> _curKeysDown;
	std::bitset<(int)TRV2KeyCode::__COUNT> _wasKeysDown;

	// Mouse wheel
	glm::vec2 _scrollWheel;

	// Mouse button
	std::bitset<(int)TRV2MouseButtonCode::__COUNT> _curMouseButtonDown;
	std::bitset<(int)TRV2MouseButtonCode::__COUNT> _wasMouseButtonDown;


	void initializeCodeMapping();
};
TRV2_NAMESPACE_END