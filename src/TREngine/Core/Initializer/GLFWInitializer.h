#pragma once
#include <memory>
#include <TREngine_Interfaces.h>
#include <Core/Core_Interfaces.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Core/GameWindow/GLFWGameWindow.h>
#include <Core/GameTimer/GLFWGameTimer.h>

TRV2_NAMESPACE_BEGIN
class GLFWInitializer : public IEngineInitializer
{
public:
	GLFWInitializer();
	~GLFWInitializer() override;

	void Initialize(const EngineSettings& config) override;

	std::shared_ptr<IGraphicsDevice> GetGraphicsDevice() const override
	{
		return _graphicsDevice;
	}
	std::shared_ptr<IGameWindow> GetGameWindow() const override
	{
		return _gameWindow;
	}
	std::shared_ptr<IGameTimer> GetGameTimer() const override
	{
		return _gameTimer;
	}

private:
	std::shared_ptr<OpenGLGraphicsDevice> _graphicsDevice;
	std::shared_ptr<GLFWGameWindow> _gameWindow;
	std::shared_ptr<GLFWGameTimer> _gameTimer;
};
TRV2_NAMESPACE_END