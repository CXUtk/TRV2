﻿#pragma once
#include <TRV2.h>
#include <string>
class ClientConfig
{
public:
	ClientConfig();

	// Game properties
	ReadonlyProperty(std::string, ClientTitle, clientTitle);
	ReadonlyProperty(std::string, VersionString, versionString);

	// Client properties
	ReadonlyProperty(int, ClientWidth, clientWidth);
	ReadonlyProperty(int, ClientHeight, clientHeight);
	ReadonlyProperty(int, FPSCap, fpsCap);

private:
};