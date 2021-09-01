#pragma once
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
class IInputController
{
public:
	virtual void ClearInput() = 0;
	virtual void UpdateInput() = 0;

	virtual glm::vec2 GetScrollValue() const = 0;

	virtual bool IsKeyJustPressed(int key) const = 0;
	virtual bool IsKeyDowned(int key) const = 0;
	virtual bool IsKeyJustReleased(int key) const = 0;

	virtual bool IsMouseClicked(int mouse) const = 0;
	virtual bool IsMouseDowned(int mouse) const = 0;
	virtual bool IsMouseReleased(int mouse) const = 0;
};
TRV2_NAMESPACE_END