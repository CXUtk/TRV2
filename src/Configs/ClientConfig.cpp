#include <Configs/ClientConfig.h>
#include <CompileTimeConfig.h>
#include <format>

ClientConfig::ClientConfig()
{
	_clientWidth = 800, _clientHeight = 600;

	_fpsCap = 60;

	_clientTitle = "TR V2";

	_versionString = std::format("{}.{}.{}.{}", TRV2_VERSION_MAJOR, 
		TRV2_VERSION_MINOR, 
		TRV2_VERSION_PATCH, 
		TRV2_VERSION_TWEAK);
}
