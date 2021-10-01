#pragma once

#include <iostream>
#include <Core.h>
#include <Platform/Platform_Interfaces.h>

TRV2_NAMESPACE_BEGIN
class WindowsFileSystem : public IFileSystem
{
public:
	WindowsFileSystem();
	~WindowsFileSystem() override;

	virtual bool IsFileExist(const std::string& path) const override;
	virtual bool IsDirectoryExist(const std::string& path) const override;

	virtual bool CreateNewDirectory(const std::string& path) const;
	virtual std::fstream Create(const std::string& name) const override;
	virtual std::ifstream OpenRead(const std::string& name, AccessMode accessMode) const override;
	virtual std::ofstream OpenWrite(const std::string& name, AccessMode accessMode) const override;

private:
};
TRV2_NAMESPACE_END