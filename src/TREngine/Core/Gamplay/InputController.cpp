#include "InputController.h"

TRV2_NAMESPACE_BEGIN
trv2::InputController::InputController() : _scrollWheel()
{}
InputController::~InputController()
{}
void InputController::ClearInput()
{}
void InputController::UpdateInput()
{}
glm::vec2 InputController::GetMousePos() const
{
	return glm::vec2();
}
TRV2_NAMESPACE_END