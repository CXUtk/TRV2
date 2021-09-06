#include "AssetsManager.h"
#include <Utils/Utils.h>
#include <Graphics/Shaders/OpenGLRawShader.h>
#include <Graphics/Shaders/OpenGLShaderProgram.h>
#include <Assets/Loaders/OpenGLTextureLoader.h>

TRV2_NAMESPACE_BEGIN
AssetsManager::AssetsManager()
{
	loadBuiltinAssets();
}

IShaderProgram* AssetsManager::GetShader(const std::string& name) const
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
	auto vs = std::make_shared<OpenGLRawShader>(ReadAllStringFromFile("Resources/Shaders/sprite2d.vert").c_str(), ShaderType::VERTEX_SHADER, "sprite2d.vert");
	auto fs = std::make_shared<OpenGLRawShader>(ReadAllStringFromFile("Resources/Shaders/sprite2d.frag").c_str(), ShaderType::FRAGMENT_SHADER, "sprite2d.frag");
	_shadersTable["builtin::sprite"] = std::make_shared<OpenGLShaderProgram>(vs, fs);

	int whitePixel = 0xffffffff;
	_texture2DTable["builtin::sprite"] = OpenGLTextureLoader::CreateTexture2DFromMemory(1, 1, (unsigned char*)&whitePixel);
}
void AssetsManager::loadTexture2D()
{

}
TRV2_NAMESPACE_END