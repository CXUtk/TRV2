#pragma once
#include <memory>
#include <TREngine_Interfaces.h>
#include <glm/glm.hpp>

TRV2_NAMESPACE_BEGIN
// Header types

enum class TRV2KeyCode
{
	TRV2_W_KEY,
	TRV2_A_KEY,
	TRV2_S_KEY,
	TRV2_D_KEY,

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

	__COUNT,
};

enum class TRV2MouseButtonCode
{
	LEFT_BUTTON,
	RIGHT_BUTTON,
	MIDDLE_BUTTON,

	__COUNT,
};

// Interfaces

class IGameWindow 
{
public:
	IGameWindow(const EngineSettings* config) {}
	virtual ~IGameWindow() = 0 {};

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual bool ShouldClose() const = 0;
	virtual void PollEvents() = 0;

	virtual const IInputController& GetInputController() const = 0;

	virtual glm::ivec2 GetWindowSize() const = 0;
};

// Input Controller
class IInputController 
{
public:
	virtual ~IInputController() = 0 {};

	virtual void ClearInput() = 0;
	virtual void UpdateInput() = 0;

	virtual glm::vec2 GetScrollValue() const = 0;

	virtual bool IsKeyJustPressed(TRV2KeyCode key) const = 0;
	virtual bool IsKeyDowned(TRV2KeyCode key) const = 0;
	virtual bool IsKeyJustReleased(TRV2KeyCode key) const = 0;

	virtual bool IsMouseClicked(TRV2MouseButtonCode mouse) const = 0;
	virtual bool IsMouseDowned(TRV2MouseButtonCode mouse) const = 0;
	virtual bool IsMouseReleased(TRV2MouseButtonCode mouse) const = 0;

	virtual glm::vec2 GetMousePos() const = 0;
};


class IEngineInitializer
{
public:
	IEngineInitializer(const EngineSettings* settings) {}
	virtual ~IEngineInitializer() = 0 {};
	
	virtual std::shared_ptr<IGraphicsDevice> GetGraphicsDevice() const = 0;
	virtual std::shared_ptr<IGameWindow> GetGameWindow() const = 0;
	virtual std::shared_ptr<IGameTimer> GetGameTimer() const = 0;
};

class IGameTimer
{
public:
	virtual ~IGameTimer() = 0 {};

	virtual double GetCurrentTime() = 0;
};
TRV2_NAMESPACE_END