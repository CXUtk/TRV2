#include "WindowsFileSystem.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <filesystem>

TRV2_NAMESPACE_BEGIN
trv2::WindowsFileSystem::WindowsFileSystem()
{}
WindowsFileSystem::~WindowsFileSystem()
{}
bool WindowsFileSystem::IsFileExist(const std::string & path) const
{
	return std::filesystem::exists(std::filesystem::path(path));
}
bool WindowsFileSystem::IsDirectoryExist(const std::string& path) const
{
	struct stat info;
	int ret;
	if(!(ret = stat(path.c_str(), &info))) return false;
	return (info.st_mode & S_IFDIR);
}
bool WindowsFileSystem::CreateNewDirectory(const std::string& path) const
{
	if (CreateDirectory(path.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		return true;
	}
	return false;
}
std::fstream WindowsFileSystem::Create(const std::string& name) const
{
	return std::fstream();
}
std::ifstream WindowsFileSystem::OpenRead(const std::string& name, AccessMode accessMode) const
{
	std::ifstream inputFile;
	inputFile.open(name.c_str(), std::ios::in | std::ios::binary);
	return inputFile;
}
std::ofstream WindowsFileSystem::OpenWrite(const std::string& name, AccessMode accessMode) const
{
	std::ofstream outputFile;
	outputFile.open(name.c_str(), std::ios::out | std::ios::app | std::ios::binary);
	return outputFile;
}
TRV2_NAMESPACE_END