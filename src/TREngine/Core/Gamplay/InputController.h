#pragma once
#include <Core.h>
#include <bitset>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN

enum class KeyCode
{
	EMPTY,

	TRV2_W_KEY,
	TRV2_A_KEY,
	TRV2_S_KEY,
	TRV2_D_KEY,
	TRV2_M_KEY,

	TRV2_UP_KEY,
	TRV2_DOWN_KEY,
	TRV2_LEFT_KEY,
	TRV2_RIGHT_KEY,

	TRV2_ENTER_KEY,
	TRV2_CTRL_KEY,
	TRV2_ALT_KEY,

	TRV2_0_KEY,
	TRV2_1_KEY,
	TRV2_2_KEY,
	TRV2_3_KEY,
	TRV2_4_KEY,
	TRV2_5_KEY,
	TRV2_6_KEY,
	TRV2_7_KEY,
	TRV2_8_KEY,
	TRV2_9_KEY,

	TRV2_PLUS_KEY,
	TRV2_MINUS_KEY,
	TRV2_TILDE_KEY,

	TRV2_SPACE_KEY,
	TRV2_ESC_KEY,

	__COUNT,
};

enum class MouseButtonCode
{
	EMPTY,

	LEFT_BUTTON,
	RIGHT_BUTTON,
	MIDDLE_BUTTON,

	__COUNT,
};


class InputController
{
public:
	InputController();
	~InputController();

	/**
	 * @brief Clear current input states and swap them to old state
	*/
	void ClearInput();

	/**
	 * @brief Calculate input state from poll events in this frame
	*/
	void DispatchInput();

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
	bool IsKeyJustPressed(KeyCode key) const { return !_wasKeysDown[(int)key] && _curKeysDown[(int)key]; }

	/**
	 * @brief Returns true if the given key is down
	 * @param key Keycode of the key
	 * @return
	*/
	bool IsKeyDowned(KeyCode key) const { return _curKeysDown[(int)key]; }

	/**
	 * @brief Returns true if the given key is released from pressed state
	 * @param key Keycode of the key
	 * @return
	*/
	bool IsKeyJustReleased(KeyCode key) const { return _wasKeysDown[(int)key] && !_curKeysDown[(int)key]; }

	/**
	 * @brief Returns true if the given mouse button is just pressed
	 * @param mouse Mouse button code
	 * @return
	*/
	bool IsMouseJustPressed(MouseButtonCode mouse) const { return !_wasMouseButtonDown[(int)mouse] && _curMouseButtonDown[(int)mouse]; }

	/**
	 * @brief Returns true if the given mouse button is pressing
	 * @param mouse
	 * @return
	*/
	bool IsMouseDowned(MouseButtonCode mouse) const { return _curMouseButtonDown[(int)mouse]; }

	/**
	 * @brief Returns true if the given mouse button is released from pressed state
	 * @param mouse
	 * @return
	*/
	bool IsMouseReleased(MouseButtonCode mouse) const { return _wasMouseButtonDown[(int)mouse] && !_curMouseButtonDown[(int)mouse]; }

	/**
	 * @brief Get current mouse position related to the bottom left point of the window
	 * @return
	*/
	glm::vec2 GetMousePos() const;

	void TriggerKeyChange(KeyCode key, bool down);
	void TriggerMouseChange(MouseButtonCode button, bool down);
	void OnScrollWheel(glm::vec2 dir);

private:

	// Keys
	std::bitset<(int)KeyCode::__COUNT> _curKeysDown;
	std::bitset<(int)KeyCode::__COUNT> _wasKeysDown;

	// Mouse wheel
	glm::vec2 _scrollWheel;

	// Mouse button
	std::bitset<(int)MouseButtonCode::__COUNT> _curMouseButtonDown;
	std::bitset<(int)MouseButtonCode::__COUNT> _wasMouseButtonDown;
};
TRV2_NAMESPACE_END