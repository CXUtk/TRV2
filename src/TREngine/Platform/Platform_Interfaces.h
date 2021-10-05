#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <fstream>
#include <string>

#include <Core.h>
#include <Core/Structures/Event.h>

TRV2_NAMESPACE_BEGIN

enum class FileMode
{
	Create,
	OpenRead,
	OpenWrite,
};

enum class AccessMode
{
	Read,
	Write,
	ReadWrite,
};

class IPlatformProvider
{
public:
	virtual ~IPlatformProvider() = 0 {};
	virtual IGameWindow* GetGameWindow() const = 0;
	virtual IFileSystem* GetFileSystem() const = 0;
};

class IGameWindow
{
public:
	virtual ~IGameWindow() = 0 {};

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual bool ShouldClose() const = 0;
	virtual void PollEvents() = 0;

	virtual glm::ivec2 GetMousePos() const = 0;
	virtual glm::ivec2 GetWindowSize() const = 0;

	virtual void AppendOnResizeEvent(Event<glm::ivec2>::func_type eventHandler) = 0;
};

class IFileSystem
{
public:
	virtual ~IFileSystem() = 0 {};

	virtual bool IsFileExist(const std::string& path) const = 0;
	virtual bool IsDirectoryExist(const std::string& path) const = 0;

	virtual std::fstream Create(const std::string& name) const = 0;
	virtual std::ifstream OpenRead(const std::string& name, AccessMode accessMode) const = 0;
	virtual std::ofstream OpenWrite(const std::string& name, AccessMode accessMode) const = 0;
};

TRV2_NAMESPACE_END