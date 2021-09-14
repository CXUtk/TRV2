#pragma once
#include <Core.h>
#include <Platform/Platform_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class GLFWProvider : public IPlatformProvider
{
public:
	GLFWProvider(const EngineSettings& settings);
	~GLFWProvider() override;

	virtual IGameWindow* GetGameWindow() const override { return trv2::ptr(_gameWindow); }

private:
	std::unique_ptr<IGameWindow> _gameWindow;
};
TRV2_NAMESPACE_END