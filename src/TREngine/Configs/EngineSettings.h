#pragma once
#include <TREngine_Interfaces.h>
#include <string>
TRV2_NAMESPACE_BEGIN
class EngineSettings
{
public:
	EngineSettings();

	// Game properties
	ReadonlyProperty(std::string, ClientTitle, clientTitle);
	ReadonlyProperty(std::string, VersionString, versionString);

	// Client properties
	ReadonlyProperty(int, ClientWidth, clientWidth);
	ReadonlyProperty(int, ClientHeight, clientHeight);
	ReadonlyProperty(int, FPSCap, fpsCap);

private:
};
TRV2_NAMESPACE_END