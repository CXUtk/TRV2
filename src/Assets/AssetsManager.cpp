#include "AssetsManager.h"
#include <Utils/Utils.h>
#include <Assets/Loaders/OpenGLShaderLoader.h>

AssetsManager::AssetsManager()
{
	_shadersTable["Sprite2D"] = OpenGLShaderLoader::LoadOpenGLShader("Resources/Shaders/sprite2d.vert",
		"Resources/Shaders/spritepure.frag");
}

std::shared_ptr<OpenGLShader> AssetsManager::GetShader(const std::string& name) const
{
	if (_shadersTable.find(name) == _shadersTable.end()) {
		throw std::exception(string_format("Cannot find shader %s", name.c_str()).c_str());
	}
	return _shadersTable.at(name);
}
