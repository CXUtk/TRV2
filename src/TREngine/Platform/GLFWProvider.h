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
	virtual IFileSystem* GetFileSystem() const override { return trv2::ptr(_fileSystem); }

private:
	std::unique_ptr<IGameWindow> _gameWindow;
	std::unique_ptr<IFileSystem> _fileSystem;
};
TRV2_NAMESPACE_END