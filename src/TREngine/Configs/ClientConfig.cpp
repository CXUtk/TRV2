﻿#include <Configs/ClientConfig.h>
#include <CompileTimeConfig_Engine.h>
#include <Utils/Utils.h>

ClientConfig::ClientConfig()
{
	_clientWidth = 800, _clientHeight = 600;

	_fpsCap = 60;

	_clientTitle = "TR V2";

	_versionString = string_format("%d.%d.%d.%d", TRV2_ENGINE_VERSION_MAJOR,
		TRV2_ENGINE_VERSION_MINOR,
		TRV2_ENGINE_VERSION_PATCH,
		TRV2_ENGINE_VERSION_TWEAK);
}