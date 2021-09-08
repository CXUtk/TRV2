#pragma once

#include <bitset>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <TREngine_Interfaces.hpp>
#include <Platform/Platform_Interfaces.hpp>
#include <TREngine/Configs/EngineSettings.hpp>

TRV2_NAMESPACE_BEGIN
class GLFWInputController
{
public:
	GLFWInputController(GLFWwindow* window, const EngineSettings& settings);
	~GLFWInputController();

	void ClearInput();
	void UpdateInput();

	void ScrollWheel(glm::vec2 dir);

	glm::vec2 GetScrollValue() const { return _scrollWheel; }

	bool IsKeyJustPressed(TRV2KeyCode key) const { return !_wasKeysDown[(int)key] && _curKeysDown[(int)key]; }
	bool IsKeyDowned(TRV2KeyCode key) const { return _curKeysDown[(int)key]; }
	bool IsKeyJustReleased(TRV2KeyCode key) const { return _wasKeysDown[(int)key] && !_curKeysDown[(int)key]; }

	bool IsMouseClicked(TRV2MouseButtonCode mouse) const { return !_wasMouseButtonDown[(int)mouse] && _curMouseButtonDown[(int)mouse]; }
	bool IsMouseDowned(TRV2MouseButtonCode mouse) const { return _curMouseButtonDown[(int)mouse]; }
	bool IsMouseReleased(TRV2MouseButtonCode mouse) const { return _wasMouseButtonDown[(int)mouse] && !_curMouseButtonDown[(int)mouse]; }

	glm::vec2 GetMousePos() const;

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