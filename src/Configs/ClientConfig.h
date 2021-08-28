#pragma once
#include <TRV2.h>
#include <string>
class ClientConfig
{
public:
	ClientConfig();

	int GetClientWidth() const { return _clientWidth; }
	int GetClientHeight() const { return _clientHeight; }
	int GetFPSCap() const { return _fpsCap; }

	ReadonlyProperty(std::string, ClientTitle, clientTitle)
	ReadonlyProperty(std::string, VersionString, versionString)

private:
	// Client render info
	int _clientWidth, _clientHeight;
	int _fpsCap;
};