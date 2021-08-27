#pragma once

class ClientConfig
{
public:
	ClientConfig();

private:
	// Client render info
	int _clientWidth, _clientHeight;

	int _fpsCap;
};