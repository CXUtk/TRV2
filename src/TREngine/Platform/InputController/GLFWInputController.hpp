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

	/**
	 * @brief Clear current input states and swap them to old state
	*/
	void ClearInput();

	/**
	 * @brief Calculate input state from poll events in this frame
	*/
	void UpdateInput();

	void ScrollWheel(glm::vec2 dir);

	/**
	 * @brief Get the change of mouse wheel value in this frame
	 * @return 
	*/
	glm::vec2 GetScrollValue() const { return _scrollWheel; }

	/**
	 * @brief Returns true if the given key is just pressed
	 * @param key Keycode of the key
	 * @return 
	*/
	bool IsKeyJustPressed(TRV2KeyCode key) const { return !_wasKeysDown[(int)key] && _curKeysDown[(int)key]; }

	/**
	 * @brief Returns true if the given key is down
	 * @param key Keycode of the key
	 * @return 
	*/
	bool IsKeyDowned(TRV2KeyCode key) const { return _curKeysDown[(int)key]; }

	/**
	 * @brief Returns true if the given key is released from pressed state
	 * @param key Keycode of the key
	 * @return 
	*/
	bool IsKeyJustReleased(TRV2KeyCode key) const { return _wasKeysDown[(int)key] && !_curKeysDown[(int)key]; }

	/**
	 * @brief Returns true if the given mouse button is just pressed
	 * @param mouse Mouse button code
	 * @return 
	*/
	bool IsMouseJustPressed(TRV2MouseButtonCode mouse) const { return !_wasMouseButtonDown[(int)mouse] && _curMouseButtonDown[(int)mouse]; }

	/**
	 * @brief Returns true if the given mouse button is pressing
	 * @param mouse 
	 * @return 
	*/
	bool IsMouseDowned(TRV2MouseButtonCode mouse) const { return _curMouseButtonDown[(int)mouse]; }

	/**
	 * @brief Returns true if the given mouse button is released from pressed state
	 * @param mouse 
	 * @return 
	*/
	bool IsMouseReleased(TRV2MouseButtonCode mouse) const { return _wasMouseButtonDown[(int)mouse] && !_curMouseButtonDown[(int)mouse]; }

	/**
	 * @brief Get current mouse position related to the bottom left point of the window
	 * @return 
	*/
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