#include "AssetsManager.h"
#include <Utils/Utils.h>
#include <Assets/Loaders/OpenGLShaderLoader.h>
#include <Assets/Loaders/OpenGLTextureLoader.h>

TRV2_NAMESPACE_BEGIN
AssetsManager::AssetsManager()
{
	loadBuiltinAssets();
}

IShader* AssetsManager::GetShader(const std::string& name) const
{
	if (_shadersTable.find(name) == _shadersTable.end()) {
		throw std::exception(string_format("Cannot find shader %s", name.c_str()).c_str());
	}
	return trv2::ptr(_shadersTable.at(name));
}

ITexture2D* AssetsManager::GetTexture2D(const std::string& name) const
{
	if (_texture2DTable.find(name) == _texture2DTable.end()) {
		throw std::exception(string_format("Cannot find texture 2d %s", name.c_str()).c_str());
	}
	return trv2::ptr(_texture2DTable.at(name));
}

void AssetsManager::loadBuiltinAssets()
{
	_shadersTable["builtin::sprite"] = OpenGLShaderLoader::CreateOpenGLShaderFromFile("Resources/Shaders/sprite2d.vert",
			"Resources/Shaders/sprite2d.frag");

	int whitePixel = 0xffffffff;
	_texture2DTable["builtin::sprite"] = OpenGLTextureLoader::CreateTexture2DFromMemory(1, 1, (unsigned char*)&whitePixel);
}
void AssetsManager::loadTexture2D()
{

}
TRV2_NAMESPACE_END