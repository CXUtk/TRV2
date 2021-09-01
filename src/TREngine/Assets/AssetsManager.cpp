#include "AssetsManager.h"
#include <Utils/Utils.h>
#include <Assets/Loaders/OpenGLShaderLoader.h>


TRV2_NAMESPACE_BEGIN
AssetsManager::AssetsManager()
{
	loadBuiltinAssets();
}

std::shared_ptr<IShader> AssetsManager::GetShader(const std::string& name) const
{
	if (_shadersTable.find(name) == _shadersTable.end()) {
		throw std::exception(string_format("Cannot find shader %s", name.c_str()).c_str());
	}
	return _shadersTable.at(name);
}

void AssetsManager::loadBuiltinAssets()
{
}
TRV2_NAMESPACE_END