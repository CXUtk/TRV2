#include "AssetsManager.h"
#include <Engine.h>

#include <Graphics/Graphics_Interfaces.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Core/Render/RawShader.h>
#include <Core/Render/ShaderProgram.h>
#include <Core/Render/Texture2D.h>
#include <Core/Utils/Utils.h>

TRV2_NAMESPACE_BEGIN

void AssetsManager::loadBuiltinAssets()
{
	auto vs = std::make_shared<RawShader>(_resourceManager, ReadAllStringFromFile("Resources/Shaders/sprite2d.vert").c_str(), ShaderType::VERTEX_SHADER, "sprite2d.vert");
	auto fs = std::make_shared<RawShader>(_resourceManager, ReadAllStringFromFile("Resources/Shaders/sprite2d.frag").c_str(), ShaderType::FRAGMENT_SHADER, "sprite2d.frag");

	auto vs_pure = std::make_shared<RawShader>(_resourceManager, ReadAllStringFromFile("Resources/Shaders/procedure.vert").c_str(), ShaderType::VERTEX_SHADER, "procedure.vert");
	auto fs_blur = std::make_shared<RawShader>(_resourceManager, ReadAllStringFromFile("Resources/Shaders/blur.frag").c_str(), ShaderType::FRAGMENT_SHADER, "blur.frag");
	auto fs_shadow = std::make_shared<RawShader>(_resourceManager, ReadAllStringFromFile("Resources/Shaders/shadowBlend.frag").c_str(), ShaderType::FRAGMENT_SHADER, "shadowBlend.frag");
	_shadersTable["builtin::sprite"] = std::make_shared<ShaderProgram>(_resourceManager, 
		trv2::cptr(vs), trv2::cptr(fs));

	_shadersTable["blur"] = std::make_shared<ShaderProgram>(_resourceManager, 
		trv2::cptr(vs_pure), trv2::cptr(fs_blur));

	_shadersTable["blendShadow"] = std::make_shared<ShaderProgram>(_resourceManager,
	trv2::cptr(vs_pure), trv2::cptr(fs_shadow));

	int whitePixel = 0xffffffff;
	_texture2DTable["builtin::sprite"] = std::make_shared<Texture2D>(_resourceManager, glm::ivec2(1), (unsigned char*)&whitePixel);

	_texture2DTable["builtin::player"] = std::make_shared<Texture2D>(_resourceManager, "Resources/Images/Player.png");

}

AssetsManager::AssetsManager(IGraphicsResourceManager* resourceManager)
	: _resourceManager(resourceManager)
{
	loadBuiltinAssets();
}

ShaderProgram* AssetsManager::GetShader(const std::string& name) const
{
	if (_shadersTable.find(name) == _shadersTable.end())
	{
		throw std::exception(string_format("Cannot find shader %s", name.c_str()).c_str());
	}
	return trv2::ptr(_shadersTable.at(name));
}

Texture2D* AssetsManager::GetTexture2D(const std::string& name) const
{
	if (_texture2DTable.find(name) == _texture2DTable.end())
	{
		throw std::exception(string_format("Cannot find texture 2d %s", name.c_str()).c_str());
	}
	return trv2::ptr(_texture2DTable.at(name));
}

void AssetsManager::loadTexture2D()
{
}
TRV2_NAMESPACE_END