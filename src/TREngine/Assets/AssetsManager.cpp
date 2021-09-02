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

std::shared_ptr<ITexture2D> AssetsManager::GetTexture2D(const std::string& name) const
{
	if (_texture2DTable.find(name) == _texture2DTable.end()) {
		throw std::exception(string_format("Cannot find texture 2d %s", name.c_str()).c_str());
	}
	return _texture2DTable.at(name);
}

void AssetsManager::loadBuiltinAssets()
{
}
TRV2_NAMESPACE_END