#pragma once
#include <string>
class ClientConfig
{
public:
	ClientConfig();

	int GetClientWidth() const { return _clientWidth; }
	int GetClientHeight() const { return _clientHeight; }
	int GetFPSCap() const { return _fpsCap; }
	std::string GetClientTitle() const { return _clientTitle; }

private:
	// Client render info
	int _clientWidth, _clientHeight;

	int _fpsCap;

	std::string _clientTitle;
};