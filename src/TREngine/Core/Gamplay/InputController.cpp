#include "InputController.h"
#include <Engine.h>
#include <Platform/Platform_Interfaces.h>

TRV2_NAMESPACE_BEGIN
InputController::InputController() : _scrollWheel()
{

}

InputController::~InputController()
{
	
}

void InputController::ClearInput()
{
	_scrollWheel = glm::vec2(0);

	_wasMouseButtonDown = _curMouseButtonDown;

	_wasKeysDown = _curKeysDown;
}

void InputController::DispatchInput()
{
	
}
	
glm::vec2 InputController::GetMousePos() const
{
	return Engine::GetInstance()->GetGameWindow()->GetMousePos();
}

void InputController::TriggerKeyChange(KeyCode key, bool down)
{
	_curKeysDown[(int)key] = down;
}

void InputController::TriggerMouseChange(MouseButtonCode button, bool down)
{
	_curMouseButtonDown[(int)button] = down;
}

void InputController::OnScrollWheel(glm::vec2 dir)
{
	_scrollWheel += dir;
}

TRV2_NAMESPACE_END