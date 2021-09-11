#pragma once
#include <Core.h>

TRV2_NAMESPACE_BEGIN
class GLFWGameWindow;
class GLFWInputController;
class GLFWGameTimer;

class _GLFWPlatform
{
public:
	using _Window_Type = GLFWGameWindow;
	using _InputController_Type = GLFWInputController;
	using _GameTimer_Type = GLFWGameTimer;

	enum { Platform = PlatformType::GLFW };

	_GLFWPlatform(const EngineSettings& settings);


	_Window_Type* GetWindow() const { return _gameWindow.get(); }
	_InputController_Type* GetInputController() const { return _inputController.get(); }
	_GameTimer_Type* GetGameTimer() const { return _gameTimer.get(); }

private:
	std::shared_ptr<_Window_Type> _gameWindow;
	std::shared_ptr<_InputController_Type> _inputController;
	std::shared_ptr<_GameTimer_Type> _gameTimer;
};

using _G_Platform = _GLFWPlatform;
TRV2_NAMESPACE_END