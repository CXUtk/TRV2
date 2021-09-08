﻿#include "AssetsManager.hpp"
#include <Utils/Utils.h>
#include <TREngine.h>
#include <Graphics/Graphics_Interfaces.h>
#include <Graphics/GraphicsDevices/OpenGLGraphicsDevice.h>
#include <Graphics/Shaders/OpenGLRawShader.h>
#include <Graphics/Shaders/OpenGLShaderProgram.h>
#include <Graphics/Textures/OpenGLTexture2D.h>

TRV2_NAMESPACE_BEGIN

AssetsManager::AssetsManager()
{
	loadBuiltinAssets();
}

void AssetsManager::loadBuiltinAssets()
{
	auto vs = std::make_shared<IRawShader>(ReadAllStringFromFile("Resources/Shaders/sprite2d.vert").c_str(), ShaderType::VERTEX_SHADER, "sprite2d.vert");
	auto fs = std::make_shared<IRawShader>(ReadAllStringFromFile("Resources/Shaders/sprite2d.frag").c_str(), ShaderType::FRAGMENT_SHADER, "sprite2d.frag");
	_shadersTable["builtin::sprite"] = std::make_shared<IShaderProgram>(vs, fs);

	auto gd = IEngine::GetInstance()->GetGraphicsDevice();
	int whitePixel = 0xffffffff;
	_texture2DTable["builtin::sprite"] = std::make_shared<ITexture2D>(gd, 1, 1, (unsigned char*)&whitePixel);
}

IShaderProgram* AssetsManager::GetShader(const std::string& name) const
{
	if (_shadersTable.find(name) == _shadersTable.end())
	{
		throw std::exception(string_format("Cannot find shader %s", name.c_str()).c_str());
	}
	return trv2::ptr(_shadersTable.at(name));
}

ITexture2D* AssetsManager::GetTexture2D(const std::string& name) const
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